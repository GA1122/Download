void RemoteFrame::DetachImpl(FrameDetachType type) {
  PluginScriptForbiddenScope forbid_plugin_destructor_scripting;
  DetachChildren();
  if (!Client())
    return;

  if (view_)
    view_->Dispose();
  GetWindowProxyManager()->ClearForClose();
  SetView(nullptr);
  ToRemoteDOMWindow(dom_window_)->FrameDetached();
  if (cc_layer_)
    SetCcLayer(nullptr, false, false);
}
