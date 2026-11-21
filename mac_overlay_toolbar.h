#ifndef CHROME_BROWSER_UI_VIEWS_MAC_OVERLAY_TOOLBAR_H_
#define CHROME_BROWSER_UI_VIEWS_MAC_OVERLAY_TOOLBAR_H_

#include <memory>

#include "ui/compositor/layer.h"
#include "ui/views/animation/bounds_animator.h"
#include "ui/views/view.h"

// ============================================================================
// ARCHITECTURE & DESIGN CONTEXT (Chromium Windowed Auto-Hide Toolbar)
// ============================================================================
// 1. Goal: Native C++ Chromium source patch implementing a macOS-style
//    auto-hiding overlay toolbar for standard *windowed* browsing mode.
// 2. Overlay vs. Push-Down: Defaults to a floating Overlay (sliding down at
//    y = 0 over a static WebView) rather than a Push-Down layout resize. This
//    eliminates HTML/CSS layout reflow lag during animations.
// 3. GPU Backdrop Sampling (Vibrancy): Instead of manual CPU/GPU resize
//    snapshotting, this view relies on ui::Layer::SetBackgroundBlur() to
//    sample and blur the live WebView content behind it natively via Skia/GPU.
// 4. Security ("Line of Death"): Rendered natively in Aura Views on a higher
//    Z-order layer than WebView so web content cannot spoof or block the UI.
// 5. TODOs for Integration:
//    - Add a 1-2px native hover hit-box at the top edge of BrowserView so
//      RevealToolbar() triggers even when this view is off-screen (-height).
//    - Patch BrowserView (browser_view.cc) to wrap TopContainerView inside this
//      container and pin WebView bounds to y = 0.
//    - Register files in chrome/browser/ui/BUILD.gn and override
//      ImmersiveModeController to prevent conflicts in windowed mode.
// ============================================================================
class MacOverlayToolbar : public views::View {
 public:
  MacOverlayToolbar();
  MacOverlayToolbar(const MacOverlayToolbar&) = delete;
  MacOverlayToolbar& operator=(const MacOverlayToolbar&) = delete;
  ~MacOverlayToolbar() override;

  // Animates the toolbar sliding down into view over the WebView.
  void RevealToolbar();

  // Animates the toolbar sliding back up off-screen to hide.
  void HideToolbar();

  // views::View overrides:
  void OnMouseEntered(const ui::MouseEvent& event) override;
  void OnMouseExited(const ui::MouseEvent& event) override;

 private:
  // Handles smooth slide animations without triggering WebView reflows.
  std::unique_ptr<views::BoundsAnimator> animator_;

  // Tracks if the toolbar is currently visible to the user.
  bool is_revealed_ = false;
};

#endif  // CHROME_BROWSER_UI_VIEWS_MAC_OVERLAY_TOOLBAR_H_
