void NewTabButton::OnGestureEvent(ui::GestureEvent* event) {
  views::ImageButton::OnGestureEvent(event);
  event->SetHandled();
}
