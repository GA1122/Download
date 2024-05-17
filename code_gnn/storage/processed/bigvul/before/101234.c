bool IsVeryFirstGetUpdates(const ClientToServerMessage& message) {
  if (!message.has_get_updates())
    return false;
  DCHECK_LT(0, message.get_updates().from_progress_marker_size());
  for (int i = 0; i < message.get_updates().from_progress_marker_size(); ++i) {
    if (!message.get_updates().from_progress_marker(i).token().empty())
      return false;
  }
  return true;
}
