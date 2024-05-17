HeadlessWebContents::Builder& HeadlessWebContents::Builder::SetAllowTabSockets(
    bool tab_sockets_allowed) {
  tab_sockets_allowed_ = tab_sockets_allowed;
  return *this;
}
