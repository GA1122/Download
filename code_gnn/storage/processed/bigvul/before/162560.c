String Resource::ReasonNotDeletable() const {
  StringBuilder builder;
  if (HasClientsOrObservers()) {
    builder.Append("hasClients(");
    builder.AppendNumber(clients_.size());
    if (!clients_awaiting_callback_.IsEmpty()) {
      builder.Append(", AwaitingCallback=");
      builder.AppendNumber(clients_awaiting_callback_.size());
    }
    if (!finished_clients_.IsEmpty()) {
      builder.Append(", Finished=");
      builder.AppendNumber(finished_clients_.size());
    }
    builder.Append(')');
  }
  if (loader_) {
    if (!builder.IsEmpty())
      builder.Append(' ');
    builder.Append("loader_");
  }
  if (IsMainThread() && GetMemoryCache()->Contains(this)) {
    if (!builder.IsEmpty())
      builder.Append(' ');
    builder.Append("in_memory_cache");
  }
  return builder.ToString();
}
