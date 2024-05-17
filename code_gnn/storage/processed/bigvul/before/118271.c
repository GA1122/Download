void AutofillDialogViews::SectionContainer::OnGestureEvent(
    ui::GestureEvent* event) {
  if (!ShouldForwardEvent(*event))
    return;

  proxy_button_->OnGestureEvent(event);
}
