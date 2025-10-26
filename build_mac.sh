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
rm -rf builds/OpusReader.app 2> /dev/null
mkdir -p builds
mv opusreader.app builds/OpusReader.app

# Copy resources to app bundle
RESOURCES_DIR="builds/OpusReader.app/Contents/Resources/resources"
mkdir -p "$RESOURCES_DIR"
cp -R pdf_viewer/shaders "$RESOURCES_DIR/"
cp pdf_viewer/opusreader.config "$RESOURCES_DIR/"
cp tutorial.pdf "$RESOURCES_DIR/"
mkdir -p "$RESOURCES_DIR/state"

# Capture the current PATH
CURRENT_PATH=$(echo $PATH)

# Define the path to the Info.plist file inside the app bundle
INFO_PLIST="resources/Info.plist"

# Add LSEnvironment key with PATH to Info.plist
/usr/libexec/PlistBuddy -c "Add :LSEnvironment dict" "$INFO_PLIST" || echo "LSEnvironment already exists"
/usr/libexec/PlistBuddy -c "Add :LSEnvironment:PATH string $CURRENT_PATH" "$INFO_PLIST" || /usr/libexec/PlistBuddy -c "Set :LSEnvironment:PATH $CURRENT_PATH" "$INFO_PLIST"

# Run macdeployqt
macdeployqt builds/OpusReader.app -dmg

# Code sign the application
echo "Signing the application..."
codesign --force --deep --sign - builds/OpusReader.app

# Create release zip
zip -r builds/opusreader-release-mac.zip builds/OpusReader.dmg

echo ""
echo "========================================="
echo "Build complete!"
echo "App: builds/OpusReader.app"
echo "DMG: builds/OpusReader.dmg"
echo "ZIP: builds/opusreader-release-mac.zip"
echo "========================================="
