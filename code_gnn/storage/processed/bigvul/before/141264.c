void Document::TasksWereUnpaused() {
  GetScriptRunner()->Resume();

  if (parser_)
    parser_->UnpauseScheduledTasks();
  if (scripted_animation_controller_)
    scripted_animation_controller_->Unpause();

  MutationObserver::ResumeSuspendedObservers();
  if (dom_window_)
    DOMWindowPerformance::performance(*dom_window_)->ResumeSuspendedObservers();
}
