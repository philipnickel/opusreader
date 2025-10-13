#!/usr/bin/env bash
set -e
# macOS-only build script for OpusReader with blur effects
# prerequisite: brew install qt@5 freeglut mesa harfbuzz

if [ -z ${MAKE_PARALLEL+x} ]; then export MAKE_PARALLEL=1; else echo "MAKE_PARALLEL defined"; fi
echo "MAKE_PARALLEL set to $MAKE_PARALLEL"

# Build mupdf
cd mupdf
make
cd ..

# Build Qt app
if [[ $1 == portable ]]; then
qmake pdf_viewer_build_config.pro
else
qmake "CONFIG+=non_portable" pdf_viewer_build_config.pro
fi

make -j$MAKE_PARALLEL

# Prepare build output directory
rm -rf builds/opusreader.app 2> /dev/null
mkdir -p builds
mv opusreader.app builds/

# Copy resources to app bundle
cp -r pdf_viewer/shaders builds/opusreader.app/Contents/MacOS/shaders
cp pdf_viewer/prefs.config builds/opusreader.app/Contents/MacOS/prefs.config
cp pdf_viewer/prefs_user.config builds/opusreader.app/Contents/MacOS/prefs_user.config 2>/dev/null || echo "No prefs_user.config"
cp pdf_viewer/keys.config builds/opusreader.app/Contents/MacOS/keys.config
cp pdf_viewer/keys_user.config builds/opusreader.app/Contents/MacOS/keys_user.config 2>/dev/null || echo "No keys_user.config"
cp tutorial.pdf builds/opusreader.app/Contents/MacOS/tutorial.pdf

# Capture the current PATH
CURRENT_PATH=$(echo $PATH)

# Define the path to the Info.plist file inside the app bundle
INFO_PLIST="resources/Info.plist"

# Add LSEnvironment key with PATH to Info.plist
/usr/libexec/PlistBuddy -c "Add :LSEnvironment dict" "$INFO_PLIST" || echo "LSEnvironment already exists"
/usr/libexec/PlistBuddy -c "Add :LSEnvironment:PATH string $CURRENT_PATH" "$INFO_PLIST" || /usr/libexec/PlistBuddy -c "Set :LSEnvironment:PATH $CURRENT_PATH" "$INFO_PLIST"

# Run macdeployqt
macdeployqt builds/opusreader.app -dmg

# Code sign the application
echo "Signing the application..."
codesign --force --deep --sign - builds/opusreader.app

# Create release zip
zip -r builds/opusreader-release-mac.zip builds/opusreader.dmg

echo ""
echo "========================================="
echo "Build complete!"
echo "App: builds/opusreader.app"
echo "DMG: builds/opusreader.dmg"
echo "ZIP: builds/opusreader-release-mac.zip"
echo "========================================="
