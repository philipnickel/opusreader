# Build Process

## Quick Reference

### Fast Development Build (No DMG)
```bash
make build
```
Builds the app and signs it automatically. **Fast** - skips DMG creation.

### Create DMG from Existing App
```bash
make bundle
```
Creates DMG from the already-built app and re-signs automatically.

### Full Clean Build with DMG
```bash
make clean && make build && make bundle
```

### Release to GitHub
```bash
make release
```
Requires DMG to exist. Auto-detects branch (main = stable, staging = pre-release).

## Complete Workflow

```bash
# 1. Clean everything
make clean

# 2. Fast build (app only)
make build

# 3. Create DMG and re-sign
make bundle

# 4. Upload to GitHub
make release
```

Or all in one line:
```bash
make clean && make build && make bundle && make release
```

## Signing

All signing happens automatically:
- `make build` signs the .app at the end
- `make bundle` re-signs after DMG creation (which invalidates the signature)

## Available Targets

- `make build` - Build app only (fast, ~30-60s)
- `make bundle` - Create DMG from app (~10-20s)
- `make release` - Upload to GitHub
- `make clean` - Remove build artifacts
- `make help` - Show all targets

## Why Separate build and bundle?

During development, you usually don't need the DMG. Separating these steps makes the build cycle much faster:

- **Without DMG**: `make build` → 30-60 seconds
- **With DMG**: `make build && make bundle` → 40-80 seconds

This saves ~10-20 seconds per build iteration.
