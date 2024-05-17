bool AutofillDialogViews::SectionContainer::ShouldForwardEvent(
    const ui::LocatedEvent& event) {
  return forward_mouse_events_ || event.y() <= child_at(0)->bounds().bottom();
}
