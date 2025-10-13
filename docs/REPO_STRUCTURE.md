# Repository Structure

```
opusreader/
├── build_mac.sh           # Build script for macOS
├── LICENSE                # License file
├── README.md              # Main readme
├── tutorial.pdf           # Tutorial PDF
│
├── builds/                # Build output (gitignored)
│   ├── opusreader.app        # Built application
│   ├── opusreader.dmg        # DMG distribution
│   └── *.zip             # Release archives
│
├── docs/                  # Documentation
│   ├── MACOS_ONLY.md     # Why this is macOS-only
│   ├── TRANSPARENCY.md   # Transparency feature docs
│   └── WINDOW_BLUR_IMPLEMENTATION.md
│
├── mupdf/                 # MuPDF library (submodule)
├── zlib/                  # Zlib library
├── pdf_viewer/            # Main source code
│   ├── *.cpp, *.h        # Source files
│   ├── shaders/          # OpenGL shaders
│   ├── prefs.config      # Default configuration
│   └── keys.config       # Default keybindings
│
├── resources/             # Application resources
├── scripts/               # Utility scripts
└── tutorial/              # Tutorial source files
```

## Build Artifacts (Not Committed)

These are generated during build and ignored by git:
- `*.o` - Object files
- `*.log` - Build logs
- `Makefile` - Generated makefile
- `moc_*` - Qt meta-object compiler files
- `.qmake.stash` - qmake cache
- `builds/*.app/` - Application bundle
