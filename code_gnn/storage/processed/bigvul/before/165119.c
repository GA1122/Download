void RemoteFrame::Detach(FrameDetachType type) {
  lifecycle_.AdvanceTo(FrameLifecycle::kDetaching);

  PluginScriptForbiddenScope forbid_plugin_destructor_scripting;
  DetachChildren();
  if (!Client())
    return;

  if (view_)
    view_->Dispose();
  GetWindowProxyManager()->ClearForClose();
  SetView(nullptr);
  ToRemoteDOMWindow(dom_window_)->FrameDetached();
  if (web_layer_)
    SetWebLayer(nullptr);
  Frame::Detach(type);
  lifecycle_.AdvanceTo(FrameLifecycle::kDetached);
}
