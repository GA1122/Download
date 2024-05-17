void RemoteFrame::Reload(FrameLoadType frame_load_type,
                         ClientRedirectPolicy client_redirect_policy) {
  Client()->Reload(frame_load_type, client_redirect_policy);
}
