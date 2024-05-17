void Document::TasksWereResumed() {
  GetScriptRunner()->Resume();

  if (parser_)
    parser_->ResumeScheduledTasks();
  if (scripted_animation_controller_)
    scripted_animation_controller_->Resume();

  MutationObserver::ResumeSuspendedObservers();
  if (dom_window_)
    DOMWindowPerformance::performance(*dom_window_)->ResumeSuspendedObservers();
}
