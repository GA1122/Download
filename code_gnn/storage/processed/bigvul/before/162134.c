void RenderProcessHostImpl::ReleaseOnCloseACK(
    RenderProcessHost* host,
    const SessionStorageNamespaceMap& sessions,
    int view_route_id) {
  DCHECK(host);
  if (sessions.empty())
    return;
  SessionStorageHolder* holder = static_cast<SessionStorageHolder*>(
      host->GetUserData(kSessionStorageHolderKey));
  if (!holder) {
    holder = new SessionStorageHolder();
    host->SetUserData(kSessionStorageHolderKey, base::WrapUnique(holder));
  }
  holder->Hold(sessions, view_route_id);
}
