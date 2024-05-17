void ShelfWidget::PostCreateShelf() {
  SetFocusCycler(Shell::Get()->focus_cycler());

  shelf_layout_manager_->LayoutShelf();
  shelf_layout_manager_->UpdateAutoHideState();
  ShowIfHidden();
}
