# Configuration Overview

OpusReader stores all user configuration in:

```
~/Library/Application Support/OpusReader/
```

## Configuration Files

### User Preferences
```
~/Library/Application Support/OpusReader/prefs_user.config
```

This file overrides default preferences. Create it to customize OpusReader behavior.

### User Keybindings
```
~/Library/Application Support/OpusReader/keys_user.config
```

This file overrides default keybindings. Create it to customize keyboard shortcuts.

### Databases
OpusReader also stores:

- `test.db` - Document state and annotations
- `local.db` - Local document settings
- `shared.db` - Shared settings (optional syncing)

## Quick Start

### Enable Transparency

Create `~/Library/Application Support/OpusReader/prefs_user.config`:

```ini
enable_transparency 1
window_transparency 0.90
macos_blur_material 23
```

### Customize Colors

```ini
custom_background_color 0.188 0.204 0.275
custom_text_color 0.776 0.816 0.961
```

### Custom Keybinding Example

Create `~/Library/Application Support/OpusReader/keys_user.config`:

```ini
# Use Cmd+T for table of contents
goto_toc <C-t>

# Use Cmd+F for search
search <C-f>
```

## Configuration Sections

- [Transparency & Blur](transparency.md) - Window effects
- [Keybindings](keybindings.md) - Keyboard shortcuts
- [Color Schemes](colors.md) - Visual customization

## Config File Format

- One setting per line
- Format: `setting_name value1 value2 ...`
- Comments start with `#`
- Values can be numbers, text, or colors (RGB 0-1 range)

## Applying Changes

Changes take effect:

- Immediately for most settings
- After reopening a document for some display settings
- After restarting OpusReader for keybindings

## Default Values

Default configuration is in the app bundle:
```
/Applications/OpusReader.app/Contents/MacOS/prefs.config
/Applications/OpusReader.app/Contents/MacOS/keys.config
```

!!! tip
    Don't edit these files - create user config files instead so your changes persist across updates.
