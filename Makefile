.PHONY: all build clean help build-core

BUILD_DIR := builds
APP_BUILD := $(BUILD_DIR)/opusreader.app
FINAL_APP := $(BUILD_DIR)/OpusReader.app
RESOURCE_DIR := $(FINAL_APP)/Contents/Resources/resources
QMAKE ?= /opt/homebrew/opt/qt@6/bin/qmake
LOG_FILE := $(BUILD_DIR)/build.log

all: build

help:
	@echo "OpusReader macOS Build"
	@echo "======================="
	@echo "make build  - compile the .app bundle"
	@echo "make clean  - remove build artifacts"

build: | $(BUILD_DIR)
	@LOG_FILE=$(LOG_FILE); \
	echo "Logging build output to $$LOG_FILE"; \
	LOG_FILE="$$LOG_FILE" /bin/bash -lc "set -o pipefail; $(MAKE) --no-print-directory build-core 2>&1 | tee \"$$LOG_FILE\""

build-core:
	@echo "Checking for MuPDF (Homebrew)..."
	@brew --prefix mupdf > /dev/null 2>&1 || { \
		echo "❌ MuPDF not found. Install via 'brew install mupdf'."; \
		exit 1; \
	}
	@rm -rf $(APP_BUILD)
	@rm -rf $(FINAL_APP)
	@rm -rf $(BUILD_DIR)/artifacts
	@echo "Generating qmake Makefile..."
	@$(QMAKE) "CONFIG+=non_portable" pdf_viewer_build_config.pro -o Makefile.qmake
	@echo "Building Qt application..."
	@$(MAKE) -f Makefile.qmake -j4
	@echo "Staging app bundle..."
	@SRC_APP=""; \
	if [ -d $(APP_BUILD) ]; then \
		SRC_APP=$(APP_BUILD); \
	elif [ -d $(BUILD_DIR)/OpusReader.app ]; then \
		SRC_APP=$(BUILD_DIR)/OpusReader.app; \
	fi; \
	if [ "$$SRC_APP" = "" ]; then \
		echo "❌ Error: App bundle not found"; \
		exit 1; \
	fi; \
	if [ "$$SRC_APP" != "$(FINAL_APP)" ]; then \
		mv "$$SRC_APP" $(FINAL_APP); \
	fi
	@mkdir -p $(RESOURCE_DIR)
	@cp pdf_viewer/opusreader.config $(RESOURCE_DIR)/
	@cp tutorial.pdf $(RESOURCE_DIR)/
	@rm -rf $(RESOURCE_DIR)/shaders
	@cp -R pdf_viewer/shaders $(RESOURCE_DIR)/
	@mkdir -p $(RESOURCE_DIR)/state
	@rm -rf $(BUILD_DIR)/intermediate
	@rm -f $(BUILD_DIR)/opusreader
	@echo ""
	@echo "========================================="
	@echo "✓ Build complete!"
	@echo "  App: $(FINAL_APP)"
	@echo "========================================="

clean:
	@echo "Cleaning build artifacts..."
	@rm -rf $(APP_BUILD)
	@rm -rf $(BUILD_DIR)/OpusReader
	@rm -rf $(FINAL_APP)
	@rm -rf $(BUILD_DIR)/intermediate
	@if [ -f Makefile.qmake ]; then $(MAKE) -f Makefile.qmake clean; fi
	@rm -f Makefile.qmake
	@echo "✓ Clean complete!"

$(BUILD_DIR):
	@mkdir -p $@
