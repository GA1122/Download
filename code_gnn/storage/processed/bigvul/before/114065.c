void TaskManagerView::Show(bool highlight_background_resources) {
  if (instance_) {
    if (instance_->highlight_background_resources_ !=
        highlight_background_resources) {
      instance_->GetWidget()->Close();
    } else {
      instance_->GetWidget()->Activate();
      return;
    }
  }
  instance_ = new TaskManagerView(highlight_background_resources);
  views::Widget::CreateWindow(instance_);
  instance_->InitAlwaysOnTopState();
  instance_->model_->StartUpdating();
  instance_->GetWidget()->Show();

  views::FocusManager* focus_manager = instance_->GetFocusManager();
  if (focus_manager)
    focus_manager->SetFocusedView(instance_->tab_table_);
}
