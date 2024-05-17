void ChromotingInstance::DidChangeFocus(bool has_focus) {
  DCHECK(plugin_task_runner_->BelongsToCurrentThread());

  if (!IsConnected())
    return;

  input_handler_.DidChangeFocus(has_focus);
  if (mouse_locker_)
    mouse_locker_->DidChangeFocus(has_focus);
}
