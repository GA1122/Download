void SessionModelAssociator::OnSessionNameInitialized(const std::string name) {
  DCHECK(CalledOnValidThread());
  if (current_session_name_.empty()) {
    current_session_name_ = name;
  }
}
