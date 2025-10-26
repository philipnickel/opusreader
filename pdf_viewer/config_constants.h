#ifndef CONFIG_CONSTANTS_H
#define CONFIG_CONSTANTS_H

// ============================================================================
// Centralized Configuration Constants
// Single source of truth for all default configuration values
// ============================================================================

namespace ConfigDefaults {

    // ========================================================================
    // Transparency and Blur Configuration
    // ========================================================================

    // Window transparency settings
    constexpr bool ENABLE_TRANSPARENCY_DEFAULT = true;
    constexpr float WINDOW_TRANSPARENCY_DEFAULT = 0.85f;  // 0.0 = opaque, 1.0 = fully transparent
    constexpr float PDF_BACKGROUND_ALPHA_DEFAULT = 0.25f;  // Opacity for PDF background

    // UI element transparency settings
    constexpr float UI_BACKGROUND_ALPHA_DEFAULT = 0.75f;  // Opacity for UI elements (status bar, input, modals)
    constexpr float UI_SELECTED_ALPHA_DEFAULT = 0.85f;    // Opacity for selected items in lists

    // macOS-specific blur settings
    constexpr int MACOS_BLUR_MATERIAL_DEFAULT = 23;  // NSVisualEffectMaterialUnderWindowBackground
    constexpr int MACOS_BLUR_AMOUNT_DEFAULT = 35;    // Blur strength (0-100)

    // macOS NSVisualEffectView parameters (previously hardcoded, now configurable)
    // NSVisualEffectBlendingMode values:
    //   0 = BehindWindow (default) - content blends with what's behind the window
    //   1 = WithinWindow - content blends with window content
    constexpr int MACOS_BLUR_BLEND_MODE_DEFAULT = 0;  // NSVisualEffectBlendingModeBehindWindow

    // NSVisualEffectState values:
    //   0 = FollowsWindowActiveState - effect follows window state
    //   1 = Active (default) - effect always active
    //   2 = Inactive - effect always inactive
    constexpr int MACOS_BLUR_STATE_DEFAULT = 1;       // NSVisualEffectStateActive

    // ========================================================================
    // Shader Configuration Constants
    // ========================================================================

    // Luminance calculation coefficients (Rec. 601 standard)
    constexpr float LUMINANCE_R = 0.299f;
    constexpr float LUMINANCE_G = 0.587f;
    constexpr float LUMINANCE_B = 0.114f;

    // Background detection thresholds for text/background separation
    // These control how the shader distinguishes between text and background
    // Lower values = more aggressive transparency on lighter areas
    // Higher values = more conservative, keeps more areas opaque
    constexpr float BG_THRESHOLD_LOW_DEFAULT = 0.3f;   // Start of transition
    constexpr float BG_THRESHOLD_HIGH_DEFAULT = 0.7f;  // End of transition

    // Dark mode uses inverted thresholds
    constexpr float DARK_MODE_BG_THRESHOLD_LOW_DEFAULT = 0.3f;
    constexpr float DARK_MODE_BG_THRESHOLD_HIGH_DEFAULT = 0.7f;

    // Blur kernel configuration
    constexpr int BLUR_KERNEL_SIZE = 5;  // 5x5 kernel

    // ========================================================================
    // Blur Amount to Kernel Radius Mapping
    // ========================================================================

    // Convert blur_amount (0-100) to blur radius for shader
    // Returns kernel radius (e.g., 2 for 5x5, 3 for 7x7, etc.)
    inline constexpr int blur_amount_to_radius(int blur_amount) {
        if (blur_amount <= 0) return 0;      // No blur
        if (blur_amount <= 25) return 1;     // 3x3 kernel
        if (blur_amount <= 50) return 2;     // 5x5 kernel (default)
        if (blur_amount <= 75) return 3;     // 7x7 kernel
        return 4;                            // 9x9 kernel (maximum)
    }

    // Get blur kernel size from radius (2*radius + 1)
    inline constexpr int blur_radius_to_size(int radius) {
        return 2 * radius + 1;
    }

    // Get total number of samples in kernel (size * size)
    inline constexpr int blur_radius_to_samples(int radius) {
        int size = blur_radius_to_size(radius);
        return size * size;
    }

} // namespace ConfigDefaults

#endif // CONFIG_CONSTANTS_H
