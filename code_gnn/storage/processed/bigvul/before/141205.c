int Document::RequestIdleCallback(
    ScriptedIdleTaskController::IdleTask* idle_task,
    const IdleRequestOptions* options) {
  return EnsureScriptedIdleTaskController().RegisterCallback(idle_task,
                                                             options);
}
