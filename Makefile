.PHONY: all build clean help release release-staging

# Default target
all: build

# Help message
help:
	@echo "OpusReader Build System"
	@echo "======================="
	@echo ""
	@echo "Available targets:"
	@echo "  make build           - Build the app and DMG"
	@echo "  make release         - Create stable release from builds/opusreader.dmg"
	@echo "  make release-staging - Create pre-release from builds/opusreader.dmg"
	@echo "  make clean           - Clean all build artifacts"
	@echo ""

# Full build
build:
	@echo "Building mupdf..."
	@cd mupdf && $(MAKE)
	@echo "Generating qmake Makefile..."
	@qmake "CONFIG+=non_portable" pdf_viewer_build_config.pro -o Makefile.qmake
	@echo "Building Qt application..."
	@$(MAKE) -f Makefile.qmake -j4
	@echo "Copying resources to app bundle..."
	@cp -r pdf_viewer/shaders builds/opusreader.app/Contents/MacOS/
	@cp pdf_viewer/prefs.config builds/opusreader.app/Contents/MacOS/
	@cp pdf_viewer/keys.config builds/opusreader.app/Contents/MacOS/
	@cp tutorial.pdf builds/opusreader.app/Contents/MacOS/
	@echo "Running macdeployqt..."
	@macdeployqt builds/opusreader.app
	@echo "Signing application..."
	@codesign --force --deep --sign - builds/opusreader.app
	@echo "Creating DMG..."
	@rm -f builds/opusreader.dmg
	@macdeployqt builds/opusreader.app -dmg
	@echo ""
	@echo "========================================="
	@echo "✓ Build complete!"
	@echo "  App: builds/opusreader.app"
	@echo "  DMG: builds/opusreader.dmg"
	@echo "========================================="

# Create stable release (requires existing DMG)
release:
	@if [ ! -f builds/opusreader.dmg ]; then \
		echo "❌ Error: builds/opusreader.dmg not found"; \
		echo "   Run 'make build' first"; \
		exit 1; \
	fi
	@echo ""
	@echo "========================================="
	@echo "Creating Stable Release"
	@echo "========================================="
	@COMMIT_COUNT=$$(git rev-list --count HEAD); \
	VERSION="v1.0.$$COMMIT_COUNT"; \
	SHORT_SHA=$$(git rev-parse --short HEAD); \
	COMMIT_MSG=$$(git log -1 --pretty=%B); \
	BRANCH=$$(git branch --show-current); \
	if [ "$$BRANCH" != "main" ]; then \
		echo "⚠️  Warning: Not on main branch (current: $$BRANCH)"; \
		read -p "Continue anyway? [y/N] " -n 1 -r; \
		echo; \
		if [[ ! $$REPLY =~ ^[Yy]$$ ]]; then \
			echo "Aborted."; \
			exit 1; \
		fi; \
	fi; \
	echo "Version: $$VERSION"; \
	echo "Commit: $$SHORT_SHA"; \
	echo ""; \
	gh release create $$VERSION \
		--title "OpusReader $$VERSION" \
		--notes "$$COMMIT_MSG" \
		--latest \
		builds/opusreader.dmg && \
	echo "" && \
	echo "✓ Release $$VERSION created!" && \
	echo "  View: https://github.com/$$(git config --get remote.origin.url | sed 's/.*://;s/.git$$//')/releases"

# Create pre-release (requires existing DMG)
release-staging:
	@if [ ! -f builds/opusreader.dmg ]; then \
		echo "❌ Error: builds/opusreader.dmg not found"; \
		echo "   Run 'make build' first"; \
		exit 1; \
	fi
	@echo ""
	@echo "========================================="
	@echo "Creating Pre-release (Staging)"
	@echo "========================================="
	@COMMIT_COUNT=$$(git rev-list --count HEAD); \
	VERSION="v1.0.$$COMMIT_COUNT-beta"; \
	SHORT_SHA=$$(git rev-parse --short HEAD); \
	COMMIT_MSG=$$(git log -1 --pretty=%B); \
	BRANCH=$$(git branch --show-current); \
	if [ "$$BRANCH" != "staging" ]; then \
		echo "⚠️  Warning: Not on staging branch (current: $$BRANCH)"; \
		read -p "Continue anyway? [y/N] " -n 1 -r; \
		echo; \
		if [[ ! $$REPLY =~ ^[Yy]$$ ]]; then \
			echo "Aborted."; \
			exit 1; \
		fi; \
	fi; \
	echo "Version: $$VERSION"; \
	echo "Commit: $$SHORT_SHA"; \
	echo ""; \
	echo "Deleting old staging-latest release..."; \
	gh release delete staging-latest --yes 2>/dev/null || true; \
	git push --delete origin staging-latest 2>/dev/null || true; \
	gh release create staging-latest \
		--title "🚧 Latest Staging Build (Pre-release)" \
		--notes "$$COMMIT_MSG\n\n---\n\n⚠️ **Pre-release build** - may contain bugs" \
		--prerelease \
		builds/opusreader.dmg && \
	echo "" && \
	echo "✓ Pre-release staging-latest updated!" && \
	echo "  View: https://github.com/$$(git config --get remote.origin.url | sed 's/.*://;s/.git$$//')/releases/tag/staging-latest"

# Clean all build artifacts (keeps DMG files)
clean:
	@echo "Cleaning build artifacts..."
	@rm -rf builds/opusreader.app builds/OpusReader.app
	@rm -rf builds/intermediate
	@rm -f builds/*.zip
	@rm -f Makefile.qmake
	@if [ -f Makefile.qmake ]; then $(MAKE) -f Makefile.qmake clean; fi
	@cd mupdf && $(MAKE) clean
	@echo "✓ Clean complete (DMG files preserved)"
