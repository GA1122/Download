void ShelfWidget::CreateStatusAreaWidget(aura::Window* status_container) {
  DCHECK(status_container);
  DCHECK(!status_area_widget_);
  status_area_widget_ =
      std::make_unique<StatusAreaWidget>(status_container, shelf_);
  status_area_widget_->Initialize();
  Shell::Get()->focus_cycler()->AddWidget(status_area_widget_.get());
  background_animator_.AddObserver(status_area_widget_.get());
  status_container->SetLayoutManager(new StatusAreaLayoutManager(this));
}
