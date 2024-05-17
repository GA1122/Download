void BaseSessionService::Save() {
  DCHECK(backend());

  if (pending_commands_.empty())
    return;

  RunTaskOnBackendThread(
      FROM_HERE,
      base::Bind(&SessionBackend::AppendCommands, backend(),
                 new std::vector<SessionCommand*>(pending_commands_),
                 pending_reset_));

  pending_commands_.clear();

  if (pending_reset_) {
    commands_since_reset_ = 0;
    pending_reset_ = false;
  }
}
