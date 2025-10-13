# Window Transparency with Background Blur

This fork of OpusReader adds support for window transparency with background blur effects, creating a "liquid glass" appearance where content behind the window is visible and blurred.

## Features

- **Window Transparency**: Configurable opacity levels for the main window
- **Background Blur (Behind Window)**: Platform-native blur effects for a frosted glass appearance
  - **macOS**: Uses NSVisualEffectView with NSVisualEffectBlendingModeBehindWindow for native vibrancy and blur
  - **Windows**: Basic transparency (future: Windows Acrylic effect)
  - **Linux**: Works with compositors like KWin or Picom

## Configuration

Add these options to your `prefs_user.config`:

```
# Enable window transparency with background blur effect
enable_transparency	1

# Window transparency/opacity level (0.0 = fully transparent, 1.0 = fully opaque)
# Recommended range: 0.85 - 0.95 for good readability with blur effect
window_transparency	0.90

# macOS only: Visual effect material type (advanced users)
# Default: 23 (UnderWindowBackground - optimal for behind-window blur)
# See "macOS Blur Materials" section below for other options
macos_blur_material	23
```

### Configuration Options

- **`enable_transparency`** (boolean, default: `0`):
  - Set to `1` to enable transparency
  - Set to `0` to disable transparency

- **`window_transparency`** (float, default: `0.90`):
  - Controls the window opacity
  - Range: `0.0` (fully transparent) to `1.0` (fully opaque)
  - Recommended values: `0.85` - `0.95` for optimal readability

- **`macos_blur_material`** (integer, macOS only, default: `23`):
  - Controls the blur material type on macOS
  - Default value `23` (UnderWindowBackground) is optimized for behind-window blur
  - See "macOS Blur Materials" section for customization options

## Platform-Specific Behavior

### macOS

On macOS, the implementation uses `NSVisualEffectView` with `NSVisualEffectBlendingModeBehindWindow` to provide native blur effects of content **behind the window**. The window will have:
- Vibrancy effect with blur behind the window (not within)
- Configurable material type for different blur appearances
- Automatic adaptation to system appearance (light/dark mode)
- Full visibility of text and UI elements

#### macOS Blur Materials

Advanced users can customize the blur appearance by setting `macos_blur_material` to different values:

| Value | Material Name | Description |
|-------|---------------|-------------|
| 0 | AppearanceBased | Adapts automatically to light/dark mode |
| 1 | Light | Light blur appearance |
| 2 | Dark | Dark blur appearance |
| 3 | Titlebar | Subtle blur like window titlebars |
| 4 | Selection | Selection highlight appearance |
| 7 | Menu | Menu-style blur |
| 8 | Popover | Popover-style blur |
| 9 | Sidebar | Sidebar appearance |
| 11 | HeaderView | Header view appearance |
| 14 | Sheet | Sheet/modal appearance |
| 15 | WindowBackground | Standard window background |
| 17 | HUDWindow | Strong dark blur (HUD style) |
| 18 | FullScreenUI | Full screen UI appearance |
| 19 | ToolTip | Tooltip appearance |
| 21 | ContentBackground | Content area background |
| **23** | **UnderWindowBackground** | **Recommended: Optimized for behind-window blur** |
| 24 | UnderPageBackground | Under page background |

**Recommended values:**
- `23` (UnderWindowBackground) - Best for most use cases
- `0` (AppearanceBased) - Good for automatic light/dark adaptation
- `21` (ContentBackground) - Alternative subtle blur

### Windows

Currently provides basic transparency. Future versions may include Windows Acrylic effect support for Windows 10/11.

### Linux

Works with compositors that support transparency and blur:
- **KWin** (KDE): Enable blur in compositor settings
- **Picom/Compton**: Configure blur in picom.conf
- **Mutter** (GNOME): May require extensions for blur effects

## Tips

1. **Readability**: Use higher opacity values (0.90-0.95) when reading text-heavy documents
2. **Dark Mode**: Transparency works particularly well with dark mode enabled
3. **Performance**: Blur effects may impact performance on older hardware
4. **Background**: Choose a relatively plain desktop background for best visual effect

## Building

Build as usual with the provided build scripts:

```bash
# macOS
./build_mac.sh

# Linux
./build_linux.sh

# Windows
build_windows.bat
```

## Example Configurations

### Subtle Transparency
```
enable_transparency	1
window_transparency	0.95
```

### Moderate Transparency (Recommended)
```
enable_transparency	1
window_transparency	0.90
```

### Higher Transparency (for visual effect)
```
enable_transparency	1
window_transparency	0.85
```

## Troubleshooting

**Issue**: No blur effect visible
- **macOS**: Ensure you're running macOS 10.10 or later
- **Linux**: Check that your compositor supports blur and it's enabled
- **All platforms**: Try adjusting the `window_transparency` value

**Issue**: Text is hard to read
- Increase the `window_transparency` value (closer to 1.0)
- Use dark mode with `toggle_dark_mode` in startup_commands
- Choose a less busy desktop background

**Issue**: Performance issues
- Disable transparency by setting `enable_transparency	0`
- Compositor blur effects can be CPU/GPU intensive

## Original OpusReader

This is a fork of [OpusReader](https://github.com/ahrm/opusreader), an excellent PDF viewer designed for reading research papers and technical books.

See the main [README.md](README.md) for general OpusReader documentation.
