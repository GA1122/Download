bool SessionModelAssociator::GetLocalSession(
    const SyncedSession* * local_session) {
  DCHECK(CalledOnValidThread());
  if (current_machine_tag_.empty())
    return false;
  *local_session =
      synced_session_tracker_.GetSession(GetCurrentMachineTag());
  return true;
}
