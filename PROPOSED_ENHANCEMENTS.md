# OpusReader - Proposed Repository Enhancements

This document outlines recommended improvements to the repository structure and setup for better maintainability, developer experience, and project sustainability.

---

## Priority 1: Critical Documentation

### 1.1 Add README.md
**Status**: Missing
**Priority**: Critical
**Effort**: Medium

Create a comprehensive README.md that includes:
- Project description and key features
- Screenshots or demo GIF
- System requirements (macOS 15.0+, Qt 6.x, mupdf, etc.)
- Installation instructions
- Quick start guide
- License information (GPL v3)
- Credits and acknowledgments

### 1.2 Add CHANGELOG.md
**Status**: Missing
**Priority**: High
**Effort**: Low

Track version changes in a standardized format:
- Current version: 2.0.0
- Follow [Keep a Changelog](https://keepachangelog.com/) format
- Document breaking changes, new features, bug fixes
- Link to git tags for each version


### 1.4 Create docs/ Directory
**Status**: Missing
**Priority**: Medium
**Effort**: Medium

Detailed documentation structure:
```
docs/
├── architecture.md          # System architecture overview
├── building.md              # Detailed build instructions
├── configuration.md         # Config file documentation
├── shaders.md               # Shader customization guide
├── scripting.md             # Python scripts documentation
└── development.md           # Development workflow
```

---

## Priority 2: Source Code Reorganization

### 2.1 Restructure pdf_viewer/ Directory
**Status**: All files flat in one directory (58 items)
**Priority**: High
**Effort**: High

**Current**: Everything in `pdf_viewer/`
**Proposed**:
```
src/
├── core/                    # Core functionality
│   ├── document.{h,cpp}
│   ├── pdf_renderer.{h,cpp}
│   ├── coordinates.{h,cpp}
│   ├── checksum.{h,cpp}
│   └── book.{h,cpp}
├── ui/                      # UI components
│   ├── main_widget.{h,cpp}
│   ├── document_view.{h,cpp}
│   ├── pdf_view_opengl_widget.{h,cpp}
│   └── ui.{h,cpp}
├── database/                # Database layer
│   ├── database.{h,cpp}
│   ├── sqlite3.{h,c}
│   └── sqlite3ext.h
├── utils/                   # Utilities
│   ├── utils.{h,cpp}
│   ├── path.{h,cpp}
│   ├── input.{h,cpp}
│   ├── config.{h,cpp}
│   └── new_file_checker.{h,cpp}
├── third_party/             # Third-party code
│   ├── utf8/
│   ├── synctex/
│   ├── rapidfuzz_amalgamated.hpp
│   └── fts_fuzzy_match.h
├── platform/                # Platform-specific code
│   ├── RunGuard.{h,cpp}
│   └── OpenWithApplication.{h,cpp}
├── shaders/                 # OpenGL shaders
├── fonts/                   # Font resources
└── main.cpp                 # Entry point
```

---

## Priority 3: Build System Improvements

### 3.1 CMake Migration
**Status**: Currently using qmake (deprecated)
**Priority**: Medium
**Effort**: High

**Rationale**:
- qmake is deprecated by Qt
- CMake is the modern Qt standard
- Better cross-platform support
- Better IDE integration
- Industry standard for C++ projects

**Migration Path**:
1. Create CMakeLists.txt alongside existing .pro file
2. Test builds with both systems
3. Update CI/CD to use CMake
4. Eventually deprecate qmake

---

## Priority 5: Testing Infrastructure

### 5.1 Add Test Suite
**Status**: No visible tests
**Priority**: Medium
**Effort**: High

**Proposed Structure**:
```
tests/
├── unit/                    # Unit tests
│   ├── test_coordinates.cpp
│   ├── test_path.cpp
│   ├── test_utils.cpp
│   └── test_checksum.cpp
├── integration/             # Integration tests
│   ├── test_document_loading.cpp
│   └── test_database.cpp
├── test_data/               # Test fixtures
│   ├── sample.pdf
│   └── test_documents/
└── CMakeLists.txt           # Test build config
```

**Framework**: Qt Test (already in Qt dependency)
---

## Priority 6: Dependency Management

### 6.1 Document Dependencies
**Status**: Dependencies mentioned in build_mac.sh comments
**Priority**: High
**Effort**: Low

Create `DEPENDENCIES.md`:
- List all dependencies with versions
- Document why each is needed
- Link to dependency documentation
- Installation instructions per platform

### 6.2 Add Package Manager Support
**Status**: Manual brew install
**Priority**: Low
**Effort**: Medium

**Options**:
- **vcpkg**: Microsoft's C++ package manager
- **Conan**: Popular alternative
- Keep Homebrew for macOS but document it better

### 6.3 Add mupdf as Submodule
**Status**: build_mac.sh references mupdf/ but it's not in repo
**Priority**: High
**Effort**: Low

Either:
- Add mupdf as git submodule
- Document how to obtain mupdf
- Or use system mupdf only (via brew)
Note: We should just use system mupdf

---

## Priority 7: Configuration Management

### 7.1 Reorganize Configuration Files
**Status**: Config files scattered in pdf_viewer/
**Priority**: Low
**Effort**: Low

**Proposed**:
```
config/
├── defaults/
│   ├── opusreader.config    # Default config
│   ├── keys.config          # Default keybindings
│   └── prefs.config         # Default preferences
└── README.md                # Configuration documentation
```

**Benefits**:
- Clear separation of defaults vs user configs
- Easier to document configuration options
- User configs stay in appropriate locations (~/Library on macOS)
---

---

## Priority 9: Version Control Enhancements

### 9.1 Add .editorconfig
**Status**: Missing
**Priority**: Low
**Effort**: Low

Ensure consistent coding style across editors:
```ini
[*]
charset = utf-8
end_of_line = lf
insert_final_newline = true
trim_trailing_whitespace = true

[*.{cpp,h}]
indent_style = tab
indent_size = 4

[*.{py,sh}]
indent_style = space
indent_size = 4

[*.pro]
indent_style = space
indent_size = 4
```

### 9.2 Add .clang-format
**Status**: Missing
**Priority**: Low
**Effort**: Low

Automated C++ code formatting configuration based on project style

### 9.3 Add .clang-tidy
**Status**: Missing
**Priority**: Low
**Effort**: Low

Static analysis configuration for catching common bugs

---

## Priority 10: Project Metadata

### 10.1 Add AUTHORS File
**Status**: Missing
**Priority**: Low
**Effort**: Low

List project contributors and maintainers

### 10.2 Add SECURITY.md
**Status**: Missing
**Priority**: Low
**Effort**: Low

Security policy and vulnerability reporting instructions

### 10.3 Enhance .gitignore
**Status**: Basic coverage
**Priority**: Low
**Effort**: Low

Ensure coverage of:
- Build artifacts (covered)
- IDE files (partially covered)
- macOS system files (covered)
- User config files (covered)
- Python cache files (add: `__pycache__/`, `*.pyc`)
- Temporary files

---

## Priority 11: Release Management

### 11.1 Improve Release Process
**Status**: Manual build and zip
**Priority**: Medium
**Effort**: Medium (with CI/CD)

**Enhancements**:
- Automated GitHub Releases via CI/CD
- Semantic versioning clearly documented
- Automated changelog generation
- Checksums for all release artifacts
- Automated code signing (currently manual)
- Release notes template

### 11.2 Version Tracking
**Status**: Version in .pro file only
**Priority**: Low
**Effort**: Low

Ensure version is tracked in:
- pdf_viewer_build_config.pro (current)
- Info.plist (current)
- Source code constant
- CHANGELOG.md

---

## Priority 12: Additional Resources

### 12.1 Add .github/ Templates
**Status**: Missing
**Priority**: Low
**Effort**: Low

```
.github/
├── ISSUE_TEMPLATE/
│   ├── bug_report.md
│   └── feature_request.md
└── pull_request_template.md
```
---

