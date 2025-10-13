# macOS Only Fork

This is a macOS-only fork of OpusReader focused on window transparency and blur effects.

## Why macOS Only?

This fork implements advanced window effects using:
- **NSVisualEffectView** for native blur
- **NSVisualEffectBlendingModeBehindWindow** for behind-window blur
- macOS-specific window management APIs
- Metal/OpenGL integration specific to macOS

These features are deeply integrated with macOS frameworks and cannot be easily ported to other platforms.

## Building

Use the provided macOS build script:
```bash
./build_mac.sh
```

Output will be in `builds/opusreader.app`

## Original OpusReader

For cross-platform versions, see the original [OpusReader repository](https://github.com/ahrm/opusreader).

This fork maintains Windows/Linux code paths in the codebase but they are not actively maintained or tested.
