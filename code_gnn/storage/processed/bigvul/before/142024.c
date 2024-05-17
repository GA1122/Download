void AddActionCallback(const ActionCallback& callback) {
  DCHECK(g_task_runner.Get());
  DCHECK(g_task_runner.Get()->BelongsToCurrentThread());
  g_callbacks.Get().push_back(callback);
}
