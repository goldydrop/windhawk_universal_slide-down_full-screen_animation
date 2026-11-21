#ifndef CHROME_BROWSER_UI_VIEWS_MAC_OVERLAY_TOOLBAR_H_
#define CHROME_BROWSER_UI_VIEWS_MAC_OVERLAY_TOOLBAR_H_

#include "ui/views/view.h"
#include "ui/compositor/layer.h"
#include "ui/views/animation/bounds_animator.h"

// A custom view that acts as an overlay container for the Chrome toolbar,
// implementing a macOS-style frosted glass blur and slide-down animation.
class MacOverlayToolbar : public views::View {
 public:
  MacOverlayToolbar();
  MacOverlayToolbar(const MacOverlayToolbar&) = delete;
  MacOverlayToolbar& operator=(const MacOverlayToolbar&) = delete;
  ~MacOverlayToolbar() override;

  // Animates the toolbar sliding down into view.
  void RevealToolbar();

  // Animates the toolbar sliding back up to hide.
  void HideToolbar();

  // views::View overrides:
  void OnMouseEntered(const ui::MouseEvent& event) override;
  void OnMouseExited(const ui::MouseEvent& event) override;

 private:
  // Handles the slide animations smoothly.
  std::unique_ptr<views::BoundsAnimator> animator_;
  
  // Tracks if the toolbar is currently visible to the user.
  bool is_revealed_ = false;
};

#endif  // CHROME_BROWSER_UI_VIEWS_MAC_OVERLAY_TOOLBAR_H_
