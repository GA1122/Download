void ShelfWidget::OnGestureEvent(ui::GestureEvent* event) {
  if (event->type() == ui::ET_GESTURE_TAP_DOWN)
    keyboard::KeyboardController::Get()->HideKeyboardImplicitlyByUser();
  views::Widget::OnGestureEvent(event);
}
