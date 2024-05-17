bool ShelfWidget::DelegateView::CanActivate() const {
  aura::Window* active_window = wm::GetActiveWindow();
  aura::Window* bubble_window = nullptr;
  aura::Window* shelf_window = shelf_widget_->GetNativeWindow();
  if (shelf_widget_->IsShowingOverflowBubble()) {
    bubble_window = shelf_widget_->shelf_view_->overflow_bubble()
                        ->bubble_view()
                        ->GetWidget()
                        ->GetNativeWindow();
  }
  if (active_window &&
      (active_window == bubble_window || active_window == shelf_window)) {
    return true;
  }

  return focus_cycler_ && focus_cycler_->widget_activating() == GetWidget();
}
