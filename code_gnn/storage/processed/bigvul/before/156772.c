inline RemoteFrame::RemoteFrame(RemoteFrameClient* client,
                                Page& page,
                                FrameOwner* owner)
    : Frame(client, page, owner, RemoteWindowProxyManager::Create(*this)),
      security_context_(RemoteSecurityContext::Create()) {
  dom_window_ = RemoteDOMWindow::Create(*this);
  UpdateInertIfPossible();
  UpdateInheritedEffectiveTouchActionIfPossible();
}
