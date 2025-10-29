#include "ui.h"
#include <algorithm>
#include <qfiledialog.h>
#include <qregularexpression.h>
#include <qpainter.h>
#include <qframe.h>
#include <QStringList>

extern std::wstring DEFAULT_OPEN_FILE_PATH;

std::wstring select_command_file_name(std::string command_name) {
	if (command_name == "open_document") {
		return select_document_file_name();
	}
	else if (command_name == "source_config") {
		return select_any_file_name();
	}
	else {
		return select_any_file_name();
	}
}

std::wstring select_document_file_name() {
	if (DEFAULT_OPEN_FILE_PATH.size() == 0) {

		QString file_name = QFileDialog::getOpenFileName(nullptr, "Select Document", "", "Documents (*.pdf *.epub *.cbz)");
		return file_name.toStdWString();
	}
	else {

		QFileDialog fd = QFileDialog(nullptr, "Select Document", "", "Documents (*.pdf *.epub *.cbz)");
		fd.setDirectory(QString::fromStdWString(DEFAULT_OPEN_FILE_PATH));
		if (fd.exec()) {
			
			QString file_name = fd.selectedFiles().first();
			return file_name.toStdWString();
		}
		else {
			return L"";
		}
	}

}

std::wstring select_json_file_name() {
	QString file_name = QFileDialog::getOpenFileName(nullptr, "Select Document", "", "Documents (*.json )");
	return file_name.toStdWString();
}

std::wstring select_any_file_name() {
	QString file_name = QFileDialog::getOpenFileName(nullptr, "Select File", "", "Any (*)");
	return file_name.toStdWString();
}

std::wstring select_new_json_file_name() {
	QString file_name = QFileDialog::getSaveFileName(nullptr, "Select Document", "", "Documents (*.json )");
	return file_name.toStdWString();
}

std::wstring select_new_pdf_file_name() {
	QString file_name = QFileDialog::getSaveFileName(nullptr, "Select Document", "", "Documents (*.pdf )");
	return file_name.toStdWString();
}


std::vector<ConfigFileChangeListener*> ConfigFileChangeListener::registered_listeners;

ConfigFileChangeListener::ConfigFileChangeListener() {
	registered_listeners.push_back(this);
}

ConfigFileChangeListener::~ConfigFileChangeListener() {
	registered_listeners.erase(std::find(registered_listeners.begin(), registered_listeners.end(), this));
}

void ConfigFileChangeListener::notify_config_file_changed(ConfigManager* new_config_manager) {
	for (auto* it : ConfigFileChangeListener::registered_listeners) {
		it->on_config_file_changed(new_config_manager);
	}
}

bool HierarchialSortFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
	// custom behaviour :
	const QRegularExpression filter_regex = filterRegularExpression();
	if (!filter_regex.pattern().isEmpty())
	{
		// get source-model index for current row
		QModelIndex source_index = sourceModel()->index(source_row, this->filterKeyColumn(), source_parent);
		if (source_index.isValid())
		{
			// check current index itself :
			QString key = sourceModel()->data(source_index, filterRole()).toString();

			bool parent_contains = key.contains(filter_regex);

			if (parent_contains) return true;

			// if any of children matches the filter, then current index matches the filter as well
			int i, nb = sourceModel()->rowCount(source_index);
			for (i = 0; i < nb; ++i)
			{
				if (filterAcceptsRow(i, source_index))
				{
					return true;
				}
			}
			return false;
		}
	}
	// parent call for initial behaviour
	return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
}

bool MySortFilterProxyModel::filterAcceptsRow(int source_row,
	const QModelIndex& source_parent) const
{
	if (FUZZY_SEARCHING) {

		QModelIndex source_index = sourceModel()->index(source_row, this->filterKeyColumn(), source_parent);
		if (source_index.isValid())
		{
			// check current index itself :

			QString key = sourceModel()->data(source_index, filterRole()).toString();
			if (filterString.size() == 0) return true;
			std::wstring s1 = filterString.toStdWString();
			std::wstring s2 = key.toStdWString();
			int score = static_cast<int>(rapidfuzz::fuzz::partial_ratio(s1, s2));

			return score > 50;
		}
		else {
			return false;
		}
	}
	else {
		return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
	}
}

void MySortFilterProxyModel::setFilterCustom(QString filterString) {
	if (FUZZY_SEARCHING) {
		this->filterString = filterString;
		this->setFilterFixedString(filterString);
		sort(0);
	}
	else {
		this->setFilterFixedString(filterString);
	}
}

bool MySortFilterProxyModel::lessThan(const QModelIndex& left,
	const QModelIndex& right) const
{
	if (FUZZY_SEARCHING) {

		QString leftData = sourceModel()->data(left).toString();
		QString rightData = sourceModel()->data(right).toString();

		int left_score = static_cast<int>(rapidfuzz::fuzz::partial_ratio(filterString.toStdWString(), leftData.toStdWString()));
		int right_score = static_cast<int>(rapidfuzz::fuzz::partial_ratio(filterString.toStdWString(), rightData.toStdWString()));
		return left_score > right_score;
	}
	else {
		return QSortFilterProxyModel::lessThan(left, right);
	}
}
 MySortFilterProxyModel::MySortFilterProxyModel() {
	 if (FUZZY_SEARCHING) {
		 setDynamicSortFilter(true);
	 }
}


LeaderMenuWidget::LeaderMenuWidget(LeaderMenuNode root_node, QWidget* parent)
	: QWidget(parent),
	root(std::move(root_node))
{
	setAttribute(Qt::WA_TranslucentBackground);
	setAutoFillBackground(false);
	setFocusPolicy(Qt::StrongFocus);

	if (parentWidget()) {
		setFixedSize(parentWidget()->size());
		move(0, 0);
	}

	QVBoxLayout* root_layout = new QVBoxLayout(this);
	root_layout->setContentsMargins(36, 24, 36, 24);
	root_layout->setSpacing(0);

	root_layout->addStretch();

	panel = new QWidget(this);
	panel->setAutoFillBackground(true);
	panel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

	QVBoxLayout* panel_layout = new QVBoxLayout(panel);
	panel_layout->setContentsMargins(18, 14, 18, 14);
	panel_layout->setSpacing(4);

	breadcrumb_label = new QLabel(panel);
	breadcrumb_label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	panel_layout->addWidget(breadcrumb_label);

	entries_container = new QWidget(panel);
	entries_grid = new QGridLayout(entries_container);
	entries_grid->setContentsMargins(12, 12, 12, 12);
	entries_grid->setHorizontalSpacing(28);
	entries_grid->setVerticalSpacing(8);
	panel_layout->addWidget(entries_container);

	root_layout->addWidget(panel, 0, Qt::AlignHCenter | Qt::AlignBottom);

	node_stack.clear();
	node_stack.push_back(&root);
	rebuild_entries();
	apply_styles();
	update_panel_geometry();
	setFocus();
}

void LeaderMenuWidget::rebuild_entries() {
	key_labels.clear();
	description_labels.clear();
	arrow_labels.clear();
	visible_entries.clear();

	if (!entries_grid || node_stack.empty()) {
		return;
	}

	while (auto item = entries_grid->takeAt(0)) {
		if (auto* widget = item->widget()) {
			delete widget;
		}
		delete item;
	}

	const auto& children = node_stack.back()->children;
	if (children.empty()) {
		update_breadcrumb();
		return;
	}

	const int max_columns = 3;
	const int entries_per_column = 8;
	int column_count = std::max(1, std::min(max_columns, (static_cast<int>(children.size()) + entries_per_column - 1) / entries_per_column));
	int rows = (static_cast<int>(children.size()) + column_count - 1) / column_count;
	if (rows <= 0) {
		rows = static_cast<int>(children.size());
	}

	for (size_t i = 0; i < children.size(); ++i) {
		const LeaderMenuNode* child = &children[i];
		visible_entries.push_back(child);

		int column = static_cast<int>(i) / rows;
		int row = static_cast<int>(i) % rows;

		QWidget* row_widget = new QWidget(entries_container);
		QHBoxLayout* row_layout = new QHBoxLayout(row_widget);
		row_layout->setContentsMargins(0, 0, 0, 0);
		row_layout->setSpacing(12);

		QString key_display = child->key.toUpper();
		QLabel* key_label = new QLabel(key_display, row_widget);
		key_label->setAlignment(Qt::AlignCenter);
		key_label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
		key_labels.push_back(key_label);

		QString arrow_text = child->children.empty() ? QStringLiteral("  ") : QStringLiteral("->");
		QLabel* arrow_label = new QLabel(arrow_text, row_widget);
		arrow_label->setAlignment(Qt::AlignCenter);
		arrow_labels.push_back(arrow_label);

		QLabel* description_label = new QLabel(child->description, row_widget);
		description_label->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
		description_labels.push_back(description_label);

		row_layout->addWidget(key_label, 0, Qt::AlignLeft);
		row_layout->addWidget(arrow_label, 0, Qt::AlignLeft);
		row_layout->addWidget(description_label, 1);

		entries_grid->addWidget(row_widget, row, column);
	}

	for (int c = 0; c < column_count; ++c) {
		entries_grid->setColumnStretch(c, 1);
	}

	update_breadcrumb();
}

void LeaderMenuWidget::apply_styles() {
	QString overlay_style = QStringLiteral("background-color: rgba(8, 10, 18, 160);");
	setStyleSheet(overlay_style);

	int base_size = 10;
	if (FONT_SIZE > 0) {
		base_size = std::max(8, FONT_SIZE - 4);
	}
	const int breadcrumb_size = std::max(8, base_size - 2);

	QString panel_style = QStringLiteral(
		"background-color: rgba(18, 19, 29, 230);"
		"border: 1px solid #2c2f40;"
		"border-radius: 10px;"
		"padding: 4px;"
	);
	panel->setStyleSheet(panel_style);

	if (entries_container) {
		entries_container->setStyleSheet(QStringLiteral(
			"background-color: rgba(23, 24, 36, 210);"
			"border-radius: 8px;"
		));
	}

	if (breadcrumb_label) {
		breadcrumb_label->setStyleSheet(QStringLiteral(
			"color: #737aa2;"
			"font-weight: 600;"
			"letter-spacing: 0.5px;"
		) + QString("font-size: %1px;").arg(breadcrumb_size));
	}

	for (auto* label : key_labels) {
		if (label) {
			label->setStyleSheet(QStringLiteral(
				"color: #c0caf5;"
				"background-color: rgba(38, 43, 65, 240);"
				"border: 1px solid #475176;"
				"border-radius: 6px;"
				"padding: 4px 9px;"
				"font-weight: 600;"
				"min-width: 32px;"
				"text-align: center;"
			) + QString("font-size: %1px;").arg(base_size));
		}
	}

	for (auto* label : arrow_labels) {
		if (label) {
			label->setStyleSheet(QStringLiteral(
				"color: #616a92;"
				"font-weight: 600;"
			) + QString("font-size: %1px;").arg(base_size));
		}
	}

	for (auto* label : description_labels) {
		if (label) {
			label->setStyleSheet(QStringLiteral(
				"color: #a9b1d6;"
				"padding-left: 6px;"
			) + QString("font-size: %1px;").arg(base_size));
		}
	}
}

void LeaderMenuWidget::on_config_file_changed(ConfigManager*) {
	apply_styles();
	update_panel_geometry();
}

void LeaderMenuWidget::keyPressEvent(QKeyEvent* event) {
	if (event->key() == Qt::Key_Backspace) {
		if (node_stack.size() > 1) {
			pop_node();
			return;
		}
	}

	if (event->key() == Qt::Key_Escape) {
		if (node_stack.size() > 1) {
			pop_node();
		}
		else {
			hide();
			if (parentWidget()) {
				parentWidget()->setFocus();
			}
		}
		return;
	}

	QString key_text = event->text().toLower();
	if (!key_text.isEmpty()) {
		for (const auto* entry : visible_entries) {
			if (entry && entry->key.toLower() == key_text) {
				activate_entry(entry);
				return;
			}
		}
	}

	QWidget::keyPressEvent(event);
}

void LeaderMenuWidget::activate_entry(const LeaderMenuNode* entry) {
	if (!entry) {
		return;
	}

	if (!entry->children.empty()) {
		enter_child_node(entry);
		return;
	}

	hide();
	if (parentWidget()) {
		parentWidget()->setFocus();
	}
	if (entry->action) {
		entry->action();
	}
}

void LeaderMenuWidget::enter_child_node(const LeaderMenuNode* child) {
	if (!child) {
		return;
	}

	node_stack.push_back(child);
	rebuild_entries();
	apply_styles();
}

void LeaderMenuWidget::pop_node() {
	if (node_stack.size() > 1) {
		node_stack.pop_back();
		rebuild_entries();
		apply_styles();
	}
}

void LeaderMenuWidget::update_breadcrumb() {
	if (!breadcrumb_label) {
		return;
	}

	QStringList parts;
	parts << QStringLiteral("SPACE");
	for (size_t i = 1; i < node_stack.size(); ++i) {
		parts << node_stack[i]->key.toUpper();
	}
	breadcrumb_label->setText(parts.join(QStringLiteral("  ›  ")));
}

void LeaderMenuWidget::paintEvent(QPaintEvent* event) {
	Q_UNUSED(event);
	QPainter painter(this);
	painter.fillRect(rect(), QColor(8, 10, 18, 160));
}

void LeaderMenuWidget::resizeEvent(QResizeEvent* event) {
	QWidget::resizeEvent(event);
	if (parentWidget()) {
		setFixedSize(parentWidget()->size());
		move(0, 0);
		update_panel_geometry();
	}
}

void LeaderMenuWidget::update_panel_geometry() {
	if (!panel || !parentWidget()) {
		return;
	}

	int parent_width = parentWidget()->width();
	int target_width = parent_width - 96;
	int max_width = 720;
	int min_width = 360;
	int final_width = std::clamp(target_width, min_width, max_width);
	panel->setFixedWidth(final_width);
}

HarpoonPickerWidget::HarpoonPickerWidget(std::vector<HarpoonItem> items,
	std::function<void(const HarpoonItem&)> on_select,
	std::function<void(const HarpoonItem&)> on_delete,
	QWidget* parent)
	: QWidget(parent),
	items(std::move(items)),
	on_select(std::move(on_select)),
	on_delete(std::move(on_delete)),
	number_timer(this)
{
	setAttribute(Qt::WA_TranslucentBackground);
	setAutoFillBackground(false);
	setFocusPolicy(Qt::StrongFocus);

	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);

	list_widget = new QListWidget(this);
	list_widget->setSelectionMode(QAbstractItemView::SingleSelection);
	list_widget->setUniformItemSizes(true);
	list_widget->setFocusPolicy(Qt::NoFocus);
	list_widget->setFrameShape(QFrame::NoFrame);
	layout->addWidget(list_widget);

	number_timer.setSingleShot(true);
	number_timer.setInterval(350);
	QObject::connect(&number_timer, &QTimer::timeout, [this]() {
		commit_pending_digits();
		});

	refresh_items();
	apply_styles();
	setFocus();
}

void HarpoonPickerWidget::apply_styles() {
	QString font_size_stylesheet = "";
	if (FONT_SIZE > 0) {
		font_size_stylesheet = QString("font-size: %1px;").arg(FONT_SIZE);
	}

	QString container_style = get_status_stylesheet(true) + font_size_stylesheet;
	setStyleSheet(container_style);

	QString list_style = QStringLiteral(
		"QListWidget { border: 0; }"
		"QListWidget::item { padding: 6px 12px; }"
	);
	list_widget->setStyleSheet(container_style + list_style +
		QStringLiteral("QListWidget::item:selected {") + get_selected_stylesheet(true) + QStringLiteral("}"));
}

void HarpoonPickerWidget::refresh_items() {
	clear_pending_digits();
	list_widget->clear();

	for (size_t i = 0; i < items.size(); ++i) {
		const auto& item = items[i];
		QString number_text = QString::number(static_cast<int>(i) + 1);
		if (i < 9) {
			number_text = QString::number(static_cast<int>(i) + 1);
		}
		else if (i == 9) {
			number_text = QStringLiteral("0");
		}
		else {
			number_text = QString::number(static_cast<int>(i) + 1);
		}

		QString label_text = QString::fromStdWString(item.label);
		QString display_text = number_text + QStringLiteral(". ") + label_text;
		auto* list_item = new QListWidgetItem(display_text);
		list_item->setData(Qt::UserRole, static_cast<int>(i));
		list_widget->addItem(list_item);
	}

	if (!items.empty()) {
		list_widget->setCurrentRow(0);
	}
}

void HarpoonPickerWidget::select_index(int index) {
	if (index >= 0 && index < list_widget->count()) {
		list_widget->setCurrentRow(index);
		list_widget->scrollToItem(list_widget->item(index), QAbstractItemView::PositionAtCenter);
	}
}

void HarpoonPickerWidget::activate_current_item() {
	activate_index(list_widget->currentRow());
}

void HarpoonPickerWidget::activate_index(int index) {
	if (index < 0 || index >= static_cast<int>(items.size())) {
		return;
	}

	clear_pending_digits();

	hide();
	if (parentWidget()) {
		parentWidget()->setFocus();
	}

	if (on_select) {
		on_select(items[static_cast<size_t>(index)]);
	}
}

void HarpoonPickerWidget::handle_digit(int value) {
	if (items.empty()) {
		return;
	}

	if (pending_digits.isEmpty()) {
		if (value == 0) {
			if (items.size() >= 10) {
				activate_index(9);
			}
			return;
		}

		pending_digits = QString::number(value);
		int index = value - 1;
		if (index >= 0 && index < static_cast<int>(items.size())) {
			select_index(index);
			if (items.size() <= 9) {
				activate_index(index);
				return;
			}
			number_timer.start();
		}
		else {
			clear_pending_digits();
		}
	}
	else {
		pending_digits.append(QString::number(value));
		bool ok = false;
		int index = pending_digits.toInt(&ok) - 1;
		if (ok && index >= 0 && index < static_cast<int>(items.size())) {
			select_index(index);
			number_timer.start();
		}
		else {
			clear_pending_digits();
			handle_digit(value);
			return;
		}
	}
}

void HarpoonPickerWidget::clear_pending_digits() {
	pending_digits.clear();
	if (number_timer.isActive()) {
		number_timer.stop();
	}
}

void HarpoonPickerWidget::commit_pending_digits() {
	if (pending_digits.isEmpty()) {
		return;
	}

	bool ok = false;
	int index = pending_digits.toInt(&ok) - 1;
	clear_pending_digits();
	if (ok && index >= 0 && index < static_cast<int>(items.size())) {
		activate_index(index);
	}
}

void HarpoonPickerWidget::handle_delete() {
	int index = list_widget->currentRow();
	if (index < 0 || index >= static_cast<int>(items.size())) {
		return;
	}

	if (on_delete) {
		on_delete(items[static_cast<size_t>(index)]);
	}

	clear_pending_digits();
	items.erase(items.begin() + index);
	refresh_items();
	if (!items.empty()) {
		select_index(std::min(index, static_cast<int>(items.size()) - 1));
	}
	else {
		hide();
		if (parentWidget()) {
			parentWidget()->setFocus();
		}
	}
}

void HarpoonPickerWidget::keyPressEvent(QKeyEvent* event) {
	switch (event->key()) {
	case Qt::Key_Escape:
		hide();
		if (parentWidget()) {
			parentWidget()->setFocus();
		}
		break;
	case Qt::Key_Return:
	case Qt::Key_Enter:
		activate_current_item();
		break;
	case Qt::Key_J:
	case Qt::Key_Down:
		select_index(std::min(list_widget->currentRow() + 1, list_widget->count() - 1));
		break;
	case Qt::Key_K:
	case Qt::Key_Up:
		select_index(std::max(list_widget->currentRow() - 1, 0));
		break;
	case Qt::Key_PageDown:
		select_index(std::min(list_widget->currentRow() + 10, list_widget->count() - 1));
		break;
	case Qt::Key_PageUp:
		select_index(std::max(list_widget->currentRow() - 10, 0));
		break;
	case Qt::Key_Delete:
	case Qt::Key_Backspace:
		handle_delete();
		break;
	default:
	{
		if (!event->text().isEmpty() && event->modifiers() == Qt::NoModifier) {
			QChar c = event->text().at(0);
			if (c.isDigit()) {
				handle_digit(c.digitValue());
				return;
			}
		}
		QWidget::keyPressEvent(event);
		break;
	}
	}
}

void HarpoonPickerWidget::resizeEvent(QResizeEvent* event) {
	QWidget::resizeEvent(event);
	if (parentWidget()) {
		int parent_width = parentWidget()->width();
		int parent_height = parentWidget()->height();
		setFixedSize(static_cast<int>(parent_width * 0.9f), parent_height);
		move(static_cast<int>(parent_width * 0.05f), 0);
		apply_styles();
	}
}

void HarpoonPickerWidget::on_config_file_changed(ConfigManager*) {
	apply_styles();
	refresh_items();
}
