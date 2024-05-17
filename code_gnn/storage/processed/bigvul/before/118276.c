void AutofillDialogViews::SectionContainer::OnMouseMoved(
    const ui::MouseEvent& event) {
  SetActive(ShouldForwardEvent(event));
}
