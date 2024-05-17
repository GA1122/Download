bool SessionModelAssociator::GetForeignTab(
    const std::string& tag,
    const SessionID::id_type tab_id,
    const SessionTab** tab) {
  DCHECK(CalledOnValidThread());
  return synced_session_tracker_.LookupSessionTab(tag, tab_id, tab);
}
