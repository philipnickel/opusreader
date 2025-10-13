# OpusReader

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
3. **First Launch**: macOS will block the app because it's not from an identified developer
   - Click "Cancel" on the warning dialog
   - Go to **System Settings → Privacy & Security**
   - Scroll down and click **"Open Anyway"** next to the OpusReader message
   - Click **"Open"** in the confirmation dialog
4. Enjoy your PDFs with blur effects!

## ✨ Features

- **Behind-window blur** using NSVisualEffectView  
- **Transparent PDF pages** with smart luminance-based alpha  
- **Fully readable text** - dark pixels stay opaque  
- **Configurable transparency** levels  
- All features from Sioyek (marks, bookmarks, smart jump, etc.)

## ⚙️ Configuration

OpusReader stores user configuration in:
```
~/Library/Application Support/OpusReader/
├── prefs_user.config     # User preferences
└── keys_user.config      # User keybindings
```

### Quick Config

Create `~/Library/Application Support/OpusReader/prefs_user.config`:
```ini
# Enable transparency with blur
enable_transparency 1
window_transparency 0.90
macos_blur_material 23

# Custom colors
custom_background_color 0.188 0.204 0.275
custom_text_color 0.776 0.816 0.961
```

Create `~/Library/Application Support/OpusReader/keys_user.config` to customize keybindings.

## 📖 Documentation

Full documentation: [opusreader.philipnickel.com](https://opusreader.philipnickel.com) _(coming soon)_

- [Transparency Guide](docs/TRANSPARENCY.md)
- [macOS Features](docs/MACOS_ONLY.md)
- [Repository Structure](docs/REPO_STRUCTURE.md)

## 🛠️ Building from Source

```bash
# Install dependencies
brew install qt@5

# Clone and build
git clone --recursive https://github.com/philipnickel/opusreader
cd opusreader
make build

# App will be in builds/opusreader.app
open builds/opusreader.app
```

## 🙏 Credits

Built on [Sioyek](https://github.com/ahrm/sioyek) by [@ahrm](https://github.com/ahrm).

**OpusReader** - Reading made beautiful on macOS 🍎✨
