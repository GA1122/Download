void AutofillPopupViewNativeViews::Hide() {
  controller_ = nullptr;

  DoHide();
}
