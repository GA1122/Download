void RenderViewImpl::PpapiPluginCancelComposition() {
  Send(new ViewHostMsg_ImeCancelComposition(routing_id()));
  ui::Range range(ui::Range::InvalidRange());
  Send(new ViewHostMsg_ImeCompositionRangeChanged(routing_id(), range));
}
