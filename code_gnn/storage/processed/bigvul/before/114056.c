void TaskManagerView::LinkClicked(views::Link* source, int event_flags) {
  DCHECK(source == about_memory_link_);
  task_manager_->OpenAboutMemory();
}
