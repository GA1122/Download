void Resource::TriggerNotificationForFinishObservers(
    base::SingleThreadTaskRunner* task_runner) {
  if (finish_observers_.IsEmpty())
    return;

  auto* new_collections = new HeapHashSet<WeakMember<ResourceFinishObserver>>(
      std::move(finish_observers_));
  finish_observers_.clear();

  task_runner->PostTask(FROM_HERE, WTF::Bind(&NotifyFinishObservers,
                                             WrapPersistent(new_collections)));

  DidRemoveClientOrObserver();
}
