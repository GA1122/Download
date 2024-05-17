  void Release(int old_route_id) {
    session_storage_namespaces_awaiting_close_->erase(old_route_id);
  }
