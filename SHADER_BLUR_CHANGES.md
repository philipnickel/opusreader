# PDF Background Blur Shader Implementation

## Overview
Modified the PDF rendering shaders to apply a Gaussian blur effect to the background while keeping text and foreground content sharp and clear.

## Changes Made

### 1. `simple.fragment` (Normal Mode)
- Added 5x5 Gaussian blur kernel
- Added `applyBlur()` function that samples surrounding pixels
- Detects background vs text using luminance thresholds
- Background pixels (luminance > 0.3-0.7) get blurred
- Text pixels (dark, luminance < 0.3) remain sharp
- Background transparency is applied to blurred pixels

### 2. `dark_mode.fragment` (Dark Mode)
- Same Gaussian blur implementation
- Adjusted for inverted colors in dark mode
- Background (dark pixels) get blurred
- Text (light pixels) remain sharp
- Properly handles the HSV color transformation for dark mode

### 3. `custom_colors.fragment` (Custom Color Mode)
- Blur applied after color transformation
- Maintains color matrix transformations
- Background detection works with custom colors

## How It Works

### Luminance-Based Detection
The shaders calculate luminance for each pixel:
```glsl
float luminance = dot(rgb, vec3(0.299, 0.587, 0.114));
```

### Background Detection
Uses `smoothstep(0.3, 0.7, luminance)` to create smooth transitions:
- Luminance < 0.3: Text (no blur)
- Luminance 0.3-0.7: Transition zone (partial blur)
- Luminance > 0.7: Background (full blur)

### Gaussian Blur
5x5 kernel with proper weights for smooth blurring:
- Samples 25 surrounding pixels
- Weighted by normalized Gaussian distribution
- Preserves edge details around text

### Transparency
- Text pixels: Full opacity (alpha = 1.0)
- Background pixels: Variable transparency based on `transparency` uniform
- Smooth transition between the two

## Result
- PDF backgrounds become transparent and blurred
- Text and foreground elements remain crisp and readable
- Works in all color modes (normal, dark, custom)
- Smooth transitions prevent harsh edges

## Performance Considerations
- 5x5 blur requires 25 texture samples per pixel
- Could be optimized with two-pass separable blur if needed
- Current implementation is simple and effective for most use cases

## Testing
1. Launch the app with transparency enabled
2. Open a PDF document
3. Background should appear blurred while text is sharp
4. Test in different color modes (normal, dark mode, custom colors)
