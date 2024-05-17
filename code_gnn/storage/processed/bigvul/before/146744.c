ScriptedIdleTaskController& Document::EnsureScriptedIdleTaskController() {
  if (!scripted_idle_task_controller_)
    scripted_idle_task_controller_ = ScriptedIdleTaskController::Create(this);
  return *scripted_idle_task_controller_;
}