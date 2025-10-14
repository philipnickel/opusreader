.PHONY: all build bundle clean help release

# Default target
all: build

# Help message
help:
	@echo "OpusReader Build System"
	@echo "======================="
	@echo ""
	@echo "Available targets:"
	@echo "  make build           - Build the app (fast, no DMG)"
	@echo "  make bundle          - Create DMG from existing .app"
	@echo "  make release         - Create release (auto-detects main/staging)"
	@echo "  make clean           - Clean all build artifacts"
	@echo ""
	@echo "Typical workflow:"
	@echo "  make clean && make build && make bundle && make release"
	@echo ""

# Fast build (app only, no DMG)
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
	@echo ""
	@echo "========================================="
	@echo "✓ Build complete!"
	@echo "  App: builds/opusreader.app"
	@echo "========================================="

# Create DMG from existing .app
bundle:
	@if [ ! -d builds/opusreader.app ]; then \
		echo "❌ Error: builds/opusreader.app not found"; \
		echo "   Run 'make build' first"; \
		exit 1; \
	fi
	@echo "Creating DMG from app bundle..."
	@rm -f builds/opusreader.dmg
	@macdeployqt builds/opusreader.app -dmg
	@echo "Re-signing application..."
	@codesign --force --deep --sign - builds/opusreader.app
	@echo ""
	@echo "========================================="
	@echo "✓ Bundle complete!"
	@echo "  DMG: builds/opusreader.dmg"
	@echo "  App: builds/opusreader.app (re-signed)"
	@echo "========================================="

# Create release (auto-detects branch)
release:
	@if [ ! -f builds/opusreader.dmg ]; then \
		echo "❌ Error: builds/opusreader.dmg not found"; \
		echo "   Run 'make build' first"; \
		exit 1; \
	fi
	@BRANCH=$$(git branch --show-current); \
	if [ "$$BRANCH" = "staging" ]; then \
		$(MAKE) release-staging-internal; \
	elif [ "$$BRANCH" = "main" ]; then \
		$(MAKE) release-main-internal; \
	else \
		echo "❌ Error: Must be on 'main' or 'staging' branch"; \
		echo "   Current branch: $$BRANCH"; \
		exit 1; \
	fi

# Internal: Create stable release from main
release-main-internal:
	@echo ""
	@echo "========================================="
	@echo "Creating Stable Release (main)"
	@echo "========================================="
	@COMMIT_COUNT=$$(git rev-list --count HEAD); \
	VERSION="v1.0.$$COMMIT_COUNT"; \
	SHORT_SHA=$$(git rev-parse --short HEAD); \
	COMMIT_MSG=$$(git log -1 --pretty=%B); \
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

# Internal: Create pre-release from staging
release-staging-internal:
	@echo ""
	@echo "========================================="
	@echo "Creating Pre-release (staging)"
	@echo "========================================="
	@COMMIT_COUNT=$$(git rev-list --count HEAD); \
	VERSION="v1.0.$$COMMIT_COUNT-beta"; \
	SHORT_SHA=$$(git rev-parse --short HEAD); \
	echo "Version: $$VERSION"; \
	echo "Commit: $$SHORT_SHA"; \
	echo ""; \
	echo "Deleting old staging-latest release..."; \
	git tag -d staging-latest 2>/dev/null || true; \
	git push --delete origin staging-latest 2>/dev/null || true; \
	gh release delete staging-latest --yes 2>/dev/null || true; \
	gh release create staging-latest \
		--title "Latest Build (Pre-release)" \
		--notes "**Experimental Build** Containing experimental features and changes. May contain bugs or incomplete features." \
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
