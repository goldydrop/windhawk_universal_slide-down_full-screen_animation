#include "chrome/browser/ui/views/mac_overlay_toolbar.h"

#include "ui/gfx/geometry/rect.h"
#include "ui/views/background.h"

MacOverlayToolbar::MacOverlayToolbar() {
  // 1. Force this view to render on its own native GPU layer.
  // Required to composite above the WebView safely (Line of Death security)
  // and enable hardware-accelerated backdrop blur without resizing WebView.
  SetPaintToLayer();

  // 2. Enable macOS-style Vibrancy (Live GPU Backdrop Sampling).
  // Setting fills_bounds_opaquely to false allows the GPU compositor to sample
  // the static WebView beneath it and apply a real-time Gaussian blur.
  layer()->SetFillsBoundsOpaquely(false);
  layer()->SetBackgroundBlur(20);  // 20px blur radius

  // Add a slight translucent tint over the blur for UI contrast/readability.
  layer()->SetColor(SkColorSetARGB(150, 255, 255, 255));  // Semi-transparent white

  // Initialize the bounds animator for smooth overlay slide transitions.
  animator_ = std::make_unique<views::BoundsAnimator>(this);
  animator_->SetAnimationDuration(base::Milliseconds(250));
}

MacOverlayToolbar::~MacOverlayToolbar() = default;

void MacOverlayToolbar::OnMouseEntered(const ui::MouseEvent& event) {
  // Native event intercept: Triggers in C++ Aura before the renderer process
  // receives mouse coordinates, preventing websites from hijacking clicks.
  if (!is_revealed_) {
    RevealToolbar();
  }
}

void MacOverlayToolbar::OnMouseExited(const ui::MouseEvent& event) {
  // Slide the overlay back up off-screen when the cursor leaves the toolbar.
  if (is_revealed_) {
    HideToolbar();
  }
}

void MacOverlayToolbar::RevealToolbar() {
  is_revealed_ = true;
  gfx::Rect target_bounds = bounds();
  target_bounds.set_y(0);  // Slide down to top edge of window (overlays WebView)
  animator_->AnimateViewTo(this, target_bounds);
}

void MacOverlayToolbar::HideToolbar() {
  is_revealed_ = false;
  gfx::Rect target_bounds = bounds();
  target_bounds.set_y(-bounds().height());  // Slide up off-screen
  animator_->AnimateViewTo(this, target_bounds);
}
