void RenderViewImpl::DidMovePlugin(
    const webkit::npapi::WebPluginGeometry& move) {
  SchedulePluginMove(move);
}
