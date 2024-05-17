void ShelfWidget::Shutdown() {
  shelf_layout_manager_->PrepareForShutdown();

  background_animator_.RemoveObserver(status_area_widget_.get());
  Shell::Get()->focus_cycler()->RemoveWidget(status_area_widget_.get());
  status_area_widget_.reset();

  background_animator_.RemoveObserver(delegate_view_);
  shelf_->RemoveObserver(this);

  Shell::Get()->focus_cycler()->RemoveWidget(this);
  SetFocusCycler(nullptr);
  RemoveObserver(this);
}
