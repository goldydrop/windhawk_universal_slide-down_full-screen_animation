#include "chrome/browser/ui/views/mac_overlay_toolbar.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/views/background.h"

MacOverlayToolbar::MacOverlayToolbar() {
  // 1. Force this view to render on its own native GPU layer.
  // This is required to draw over the WebView safely (Line of Death).
  SetPaintToLayer();
  
  // 2. Enable macOS-style Vibrancy (Frosted Glass Blur).
  // We tell the layer it is transparent, then ask the GPU to blur whatever is behind it.
  layer()->SetFillsBoundsOpaquely(false);
  layer()->SetBackgroundBlur(20); // 20px blur radius
  
  // Optional: Add a slight translucent tint over the blur for readability.
  layer()->SetColor(SkColorSetARGB(150, 255, 255, 255)); // Semi-transparent white

  // Initialize the animator for the slide down/up effects.
  animator_ = std::make_unique<views::BoundsAnimator>(this);
  animator_->SetAnimationDuration(base::Milliseconds(250));
}

MacOverlayToolbar::~MacOverlayToolbar() = default;

void MacOverlayToolbar::OnMouseEntered(const ui::MouseEvent& event) {
  // Native intercept: When the mouse hits the top pixels of the window,
  // this triggers before the website knows the mouse is there.
  if (!is_revealed_) {
    RevealToolbar();
  }
}

void MacOverlayToolbar::OnMouseExited(const ui::MouseEvent& event) {
  // When the mouse leaves the toolbar area, slide it back up.
  if (is_revealed_) {
    HideToolbar();
  }
}

void MacOverlayToolbar::RevealToolbar() {
  is_revealed_ = true;
  gfx::Rect target_bounds = bounds();
  target_bounds.set_y(0); // Move down to visible area
  animator_->AnimateViewTo(this, target_bounds);
}

void MacOverlayToolbar::HideToolbar() {
  is_revealed_ = false;
  gfx::Rect target_bounds = bounds();
  target_bounds.set_y(-bounds().height()); // Move up off-screen
  animator_->AnimateViewTo(this, target_bounds);
}
