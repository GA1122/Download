void BaseSessionService::ScheduleCommand(SessionCommand* command) {
  DCHECK(command);
  commands_since_reset_++;
  pending_commands_.push_back(command);
  StartSaveTimer();
}
