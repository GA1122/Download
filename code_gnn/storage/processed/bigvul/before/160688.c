void RenderFrameImpl::OnStopFinding(StopFindAction action) {
  blink::WebPlugin* plugin = GetWebPluginForFind();
  if (plugin) {
    plugin->StopFind();
    return;
  }

  frame_->StopFinding(static_cast<WebLocalFrame::StopFindAction>(action));
}
