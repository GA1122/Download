void ShelfWidget::OnMouseEvent(ui::MouseEvent* event) {
  if (event->type() == ui::ET_MOUSE_PRESSED)
    keyboard::KeyboardController::Get()->HideKeyboardImplicitlyByUser();
  views::Widget::OnMouseEvent(event);
}
