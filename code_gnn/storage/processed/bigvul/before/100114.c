void BrowserActionsContainer::HidePopup() {
  if (popup_) {
    task_factory_.RevokeAll();

    ExtensionPopup* closing_popup = popup_;
    BrowserActionButton* closing_button = popup_button_;
    popup_ = NULL;
    popup_button_ = NULL;

    closing_popup->DetachFromBrowser();
    delete closing_popup;
    closing_button->SetButtonNotPushed();
    return;
  }
}
