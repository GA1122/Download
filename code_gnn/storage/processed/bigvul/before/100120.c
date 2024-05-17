void BrowserActionsContainer::OnBrowserActionExecuted(
    BrowserActionButton* button) {
  ExtensionAction* browser_action = button->browser_action();

  if (button->IsPopup()) {
    bool same_showing = popup_ && button == popup_button_;

    HidePopup();

    if (same_showing)
      return;

    gfx::Point origin;
    View::ConvertPointToScreen(button, &origin);
    gfx::Rect rect = button->bounds();
    rect.set_x(origin.x());
    rect.set_y(origin.y());

    gfx::NativeWindow frame_window =
        toolbar_->browser()->window()->GetNativeHandle();
    BubbleBorder::ArrowLocation arrow_location = UILayoutIsRightToLeft() ?
        BubbleBorder::TOP_LEFT : BubbleBorder::TOP_RIGHT;
    popup_ = ExtensionPopup::Show(browser_action->popup_url(),
                                  toolbar_->browser(),
                                  toolbar_->browser()->profile(),
                                  frame_window,
                                  rect,
                                  arrow_location,
                                  true);   
    popup_->set_delegate(this);
    popup_button_ = button;
    popup_button_->SetButtonPushed();
    return;
  }

  ExtensionBrowserEventRouter::GetInstance()->BrowserActionExecuted(
      profile_, browser_action->extension_id(), toolbar_->browser());
}
