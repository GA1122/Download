bool AutofillDialogViews::SectionContainer::OnMousePressed(
    const ui::MouseEvent& event) {
  if (!ShouldForwardEvent(event))
    return false;

  return proxy_button_->OnMousePressed(ProxyEvent(event));
}
