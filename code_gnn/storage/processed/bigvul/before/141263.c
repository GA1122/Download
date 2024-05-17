void Document::TasksWerePaused() {
  GetScriptRunner()->Suspend();

  if (parser_)
    parser_->PauseScheduledTasks();
  if (scripted_animation_controller_)
    scripted_animation_controller_->Pause();
}
