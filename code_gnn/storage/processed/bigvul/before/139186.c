void RenderProcessHostImpl::OnCloseACK(int old_route_id) {
  SessionStorageHolder* holder =
      static_cast<SessionStorageHolder*>(GetUserData(kSessionStorageHolderKey));
  if (!holder)
    return;
  holder->Release(old_route_id);
}
