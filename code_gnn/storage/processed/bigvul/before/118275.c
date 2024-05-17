void AutofillDialogViews::SectionContainer::OnMouseExited(
    const ui::MouseEvent& event) {
  SetActive(false);
  if (!ShouldForwardEvent(event))
    return;

  proxy_button_->OnMouseExited(ProxyEvent(event));
  SchedulePaint();
}
