void WebContentsViewAura::PrepareOverscrollWindow() {
  if (overscroll_window_) {
    base::AutoReset<OverscrollMode> reset_state(&current_overscroll_gesture_,
                                                current_overscroll_gesture_);
    scoped_ptr<aura::Window> reset_window(overscroll_window_.release());
  }

  OverscrollWindowDelegate* overscroll_delegate = new OverscrollWindowDelegate(
      web_contents_,
      current_overscroll_gesture_);
  overscroll_window_.reset(new aura::Window(overscroll_delegate));
  overscroll_window_->SetType(ui::wm::WINDOW_TYPE_CONTROL);
  overscroll_window_->SetTransparent(true);
  overscroll_window_->Init(aura::WINDOW_LAYER_TEXTURED);
  overscroll_window_->layer()->SetMasksToBounds(false);
  overscroll_window_->SetName("OverscrollOverlay");

  overscroll_change_brightness_ = overscroll_delegate->has_image();
  window_->AddChild(overscroll_window_.get());

  gfx::Rect bounds = gfx::Rect(window_->bounds().size());
  if (ShouldNavigateForward(web_contents_->GetController(),
                            current_overscroll_gesture_)) {
    bounds.Offset(base::i18n::IsRTL() ? -bounds.width() : bounds.width(), 0);
  }

  aura::Window* animate_window = GetWindowToAnimateForOverscroll();
  if (animate_window == overscroll_window_)
    window_->StackChildAbove(overscroll_window_.get(), GetContentNativeView());
  else
    window_->StackChildBelow(overscroll_window_.get(), GetContentNativeView());

  UpdateOverscrollWindowBrightness(0.f);

  overscroll_window_->SetBounds(bounds);
  overscroll_window_->Show();

  overscroll_shadow_.reset(new ShadowLayerDelegate(animate_window->layer()));
}
