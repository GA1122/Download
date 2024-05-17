void SessionModelAssociator::DisassociateForeignSession(
    const std::string& foreign_session_tag) {
  DCHECK(CalledOnValidThread());
  synced_session_tracker_.DeleteSession(foreign_session_tag);
}
