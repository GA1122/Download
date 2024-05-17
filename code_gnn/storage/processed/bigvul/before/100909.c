void TaskManagerHandler::EnableTaskManager(const ListValue* indexes) {
  if (is_enabled_)
    return;

  is_enabled_ = true;
  model_->AddObserver(this);
  model_->StartUpdating();

  content::NotificationService::current()->Notify(
      chrome::NOTIFICATION_TASK_MANAGER_WINDOW_READY,
      content::Source<TaskManagerModel>(model_),
      content::NotificationService::NoDetails());
}
