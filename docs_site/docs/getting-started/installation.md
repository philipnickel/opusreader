# Installation

## Download

Download the latest version of OpusReader:

- **Stable**: [Latest Release](https://github.com/philipnickel/opusreader/releases/latest)
- **Beta**: [Pre-release](https://github.com/philipnickel/opusreader/releases/tag/staging-latest)

## Install Steps

### 1. Download the DMG

Click the DMG file from the releases page to download.

### 2. Open the DMG

Double-click the downloaded `opusreader.dmg` file to mount it.

### 3. Drag to Applications

Drag the OpusReader app icon to your Applications folder.

### 4. First Launch (Important!)

macOS will block the app on first launch because it's not from an identified developer.

!!! warning "Security Warning Expected"
    This is normal for apps distributed outside the Mac App Store.

**To open OpusReader:**

1. Try to open OpusReader from Applications
2. macOS will show: *"OpusReader cannot be opened because it is from an unidentified developer"*
3. Click **Cancel**
4. Go to **System Settings → Privacy & Security**
5. Scroll down to the Security section
6. You'll see: *"OpusReader was blocked from use because it is not from an identified developer"*
7. Click **Open Anyway**
8. Click **Open** in the confirmation dialog

!!! tip
    You only need to do this once. After the first launch, OpusReader will open normally.

## Uninstallation

To uninstall OpusReader:

1. Quit OpusReader if it's running
2. Delete OpusReader from Applications
3. (Optional) Delete configuration files:
   ```bash
   rm -rf ~/Library/Application\ Support/OpusReader
   ```

## Next Steps

- [First Launch Guide](first-launch.md)
- [Basic Usage](basic-usage.md)
- [Configuration](../configuration/overview.md)
