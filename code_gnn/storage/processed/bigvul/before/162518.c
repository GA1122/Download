void Resource::AddFinishObserver(ResourceFinishObserver* client,
                                 base::SingleThreadTaskRunner* task_runner) {
  CHECK(!is_add_remove_client_prohibited_);
  DCHECK(!finish_observers_.Contains(client));

  WillAddClientOrObserver();
  finish_observers_.insert(client);
  if (IsLoaded())
    TriggerNotificationForFinishObservers(task_runner);
}
