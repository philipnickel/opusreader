# Development Workflow

## Release Process

OpusReader uses a simple, automated release workflow.

### Quick Release

```bash
# 1. Make your changes
vim pdf_viewer/main_widget.cpp
vim docs_site/docs/features/blur.md  # Update docs if needed

# 2. Commit and push
git add .
git commit -m "Add new blur feature"
git push origin main

# 3. Build
make build

# 4. Release
make release
```

That's it! This will:
- ✅ Create a GitHub release with the DMG
- ✅ Auto-version as v1.0.X (based on commit count)
- ✅ Deploy documentation automatically
- ✅ Update "latest" docs

### What Happens

1. **`make build`**
   - Builds mupdf library
   - Compiles Qt application
   - Creates app bundle
   - Signs the app
   - Packages into DMG

2. **`make release`**
   - Creates git tag (v1.0.X)
   - Creates GitHub release
   - Uploads DMG file
   - Uses commit message as release notes
   - Triggers docs deployment (via GitHub Actions)

3. **GitHub Actions** (automatic)
   - Deploys docs to GitHub Pages
   - Creates versioned docs (e.g., v1.0.1)
   - Updates "latest" docs

### Version Numbering

Versions are auto-generated from commit count:
- Format: `v1.0.{commit-count}`
- Example: v1.0.42, v1.0.43, etc.

### Documentation

Documentation lives in `docs_site/docs/`:
```
docs_site/docs/
├── index.md                     # Home page
├── getting-started/
│   └── installation.md          # Install guide
├── configuration/
│   └── overview.md              # Config guide
└── features/                    # Feature docs (add more here)
```

**Update docs before releasing!** They're deployed automatically with each release.

### Commands

```bash
make build           # Build app and DMG
make release         # Create stable release
make clean           # Clean build artifacts (keeps DMG)
make help            # Show all commands
```

### Pre-Release Testing (Optional)

Use the `staging` branch for testing:

```bash
# Create feature branch
git checkout -b feature/new-thing

# Make changes and test
make build
open builds/opusreader.app

# When ready, merge to staging for testing
git checkout staging
git merge feature/new-thing
git push origin staging

# Test thoroughly, then merge to main for release
git checkout main
git merge staging
git push origin main
make build
make release
```

### Commit Messages

Write good commit messages - they become your release notes!

**Good:**
```bash
git commit -m "Add customizable blur intensity

Allows users to adjust blur strength via prefs.config
New setting: blur_intensity (0.0-1.0)"
```

**Bad:**
```bash
git commit -m "fix stuff"
```

### Tips

1. **Push to main first** - Updates "latest" docs immediately
2. **Then make release** - Creates versioned release
3. **Use descriptive commits** - They appear in release notes
4. **Update docs** - Document new features before releasing
5. **Test locally** - Always run `make build` and test the app first

### Troubleshooting

**Build fails?**
```bash
make clean
make build
```

**Release fails?**
- Check you're on `main` branch
- Ensure `builds/opusreader.dmg` exists
- Check GitHub CLI is authenticated: `gh auth status`

**Docs not deploying?**
- Check GitHub Actions: https://github.com/philipnickel/opusreader/actions
- Ensure GitHub Pages is enabled in repo settings

### Links

- Releases: https://github.com/philipnickel/opusreader/releases
- Documentation: https://philipnickel.github.io/opusreader/
- GitHub Actions: https://github.com/philipnickel/opusreader/actions
