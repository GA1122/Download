bool SessionModelAssociator::GetForeignSession(
    const std::string& tag,
    std::vector<SessionWindow*>* windows) {
  DCHECK(CalledOnValidThread());
  return synced_session_tracker_.LookupSessionWindows(tag, windows);
}
