bool NewTabButton::ShouldUseNativeFrame() const {
  return GetWidget() &&
    GetWidget()->GetTopLevelWidget()->ShouldUseNativeFrame();
}
