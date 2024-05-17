void AutofillDialogViews::SectionContainer::OnMouseReleased(
    const ui::MouseEvent& event) {
  if (!ShouldForwardEvent(event))
    return;

  proxy_button_->OnMouseReleased(ProxyEvent(event));
}
