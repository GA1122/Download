void RenderView::DidMovePlugin(const webkit::npapi::WebPluginGeometry& move) {
  SchedulePluginMove(move);
}
