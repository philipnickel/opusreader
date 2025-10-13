# Release Guide

## Simple Release Workflow

OpusReader uses `gh` CLI to create releases from locally-built DMG files.

### Prerequisites

- GitHub CLI installed: `brew install gh`
- Authenticated: `gh auth login`

## Quick Start

```bash
# 1. Build locally
make build

# 2. Create release
make release              # Stable release from main
make release-staging      # Pre-release from staging
```

That's it! The Makefile handles everything.

## Development Workflow

### Making Changes

```bash
# Create feature branch
git checkout -b feature/my-feature

# Make changes
vim pdf_viewer/main_widget.cpp

# Commit
git add .
git commit -m "Add new feature"
```

### Testing (Pre-release)

```bash
# Merge to staging
git checkout staging
git merge feature/my-feature

# Build
make clean
make build

# Create pre-release
make release-staging
```

This creates/updates the `staging-latest` pre-release at:
https://github.com/philipnickel/opusreader/releases/tag/staging-latest

### Releasing (Stable)

```bash
# Merge staging to main
git checkout main
git merge staging

# Build  
make clean
make build

# Create stable release
make release
```

This creates a new versioned release (v2.0.X) at:
https://github.com/philipnickel/opusreader/releases

## Commands

### Build
```bash
make build    # Build app and DMG
make clean    # Clean build artifacts (keeps DMG)
make help     # Show available commands
```

### Release
```bash
make release          # Create stable release (from main)
make release-staging  # Create/update pre-release (from staging)
```

## Version Numbering

- **Stable releases**: `v2.0.{commit-count}` (e.g., v2.0.125)
- **Pre-releases**: `v2.0.{commit-count}-beta` (e.g., v2.0.125-beta)
- Automatically calculated from git history

## Branch Strategy

- **`main`**: Production-ready code → stable releases
- **`staging`**: Testing branch → pre-releases
- **Feature branches**: Development work

## Example Session

```bash
# Start feature
git checkout -b feature/blur-fix
vim pdf_viewer/main_widget.cpp
git commit -am "Fix blur on dark mode"

# Test in staging
git checkout staging
git merge feature/blur-fix
make build
make release-staging
# → Test download from staging-latest

# Release when stable
git checkout main  
git merge staging
make build
make release
# → Creates v2.0.X
```

## Release URLs

- **Latest Stable**: https://github.com/philipnickel/opusreader/releases/latest
- **Latest Beta**: https://github.com/philipnickel/opusreader/releases/tag/staging-latest
- **All Releases**: https://github.com/philipnickel/opusreader/releases

## Best Practices

1. **Always build before releasing**: `make build` first
2. **Test in staging**: Create pre-release before stable
3. **Keep staging clean**: Only merge tested features
4. **Descriptive commits**: They show up in release notes
5. **One feature per commit**: Makes history cleaner

## Troubleshooting

**"builds/opusreader.dmg not found"**
```bash
make build  # Build first
```

**"Not on main/staging branch"**
```bash
git checkout main  # Switch to correct branch
# or press 'y' to override warning
```

**gh CLI not authenticated**
```bash
gh auth login
```
