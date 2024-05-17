void RecordComputedAction(const std::string& action) {
  TRACE_EVENT_INSTANT1("ui", "UserEvent", TRACE_EVENT_SCOPE_GLOBAL, "action", action);
  if (!g_task_runner.Get()) {
    DCHECK(g_callbacks.Get().empty());
    return;
  }

  if (!g_task_runner.Get()->BelongsToCurrentThread()) {
    g_task_runner.Get()->PostTask(FROM_HERE,
                                  BindOnce(&RecordComputedAction, action));
    return;
  }

  for (const ActionCallback& callback : g_callbacks.Get()) {
    callback.Run(action);
  }
}
