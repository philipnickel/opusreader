# OpusReader

[![Build Status](https://github.com/philipnickel/opusreader/actions/workflows/release.yml/badge.svg)](https://github.com/philipnickel/opusreader/actions)
[![Latest Release](https://img.shields.io/github/v/release/philipnickel/opusreader)](https://github.com/philipnickel/opusreader/releases/latest)
[![Latest Pre-release](https://img.shields.io/github/v/release/philipnickel/opusreader?include_prereleases&label=pre-release)](https://github.com/philipnickel/opusreader/releases/tag/staging-latest)

A beautiful, modern PDF viewer for macOS with native window blur effects and transparency.

**OpusReader** is a macOS-exclusive fork of [Sioyek](https://github.com/ahrm/sioyek), redesigned with a focus on aesthetics and the macOS experience.

## 📦 Download

- **Stable**: [Latest Release](https://github.com/philipnickel/opusreader/releases/latest)
- **Beta**: [Pre-release](https://github.com/philipnickel/opusreader/releases/tag/staging-latest)

### Installation

1. Download the DMG file from the releases page
2. Open the DMG and drag OpusReader to your Applications folder
3. Right-click OpusReader and select "Open" the first time to bypass Gatekeeper
4. Enjoy your PDFs with blur effects!

## ✨ Features

- **Behind-window blur** using NSVisualEffectView  
- **Transparent PDF pages** with smart luminance-based alpha  
- **Fully readable text** - dark pixels stay opaque  
- **Configurable transparency** levels  
- All features from OpusReader (marks, bookmarks, smart jump, etc.)

## 💻 Quick Start

```bash
# Install dependencies
brew install qt@5

# Clone and build
git clone --recursive https://github.com/YOUR_USERNAME/opusreader
cd opusreader
export PATH="/opt/homebrew/opt/qt@5/bin:$PATH"
./build_mac.sh

# App will be in builds/opusreader.app
open builds/opusreader.app
```

## ⚙️ Configuration

Transparency settings in prefs.config:
```ini
enable_transparency1
window_transparency0.90
macos_blur_material23
```

## 📖 Documentation

- [macOS Features](docs/MACOS_ONLY.md)
- [Transparency Guide](docs/TRANSPARENCY.md)
- [Repository Structure](docs/REPO_STRUCTURE.md)

## 🙏 Credits

Built on [OpusReader](https://github.com/ahrm/opusreader) by [@ahrm](https://github.com/ahrm).

**OpusReader** - Reading made beautiful on macOS 🍎✨
