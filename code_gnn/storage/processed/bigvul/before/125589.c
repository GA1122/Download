void RenderViewHostImpl::DisableScrollbarsForThreshold(const gfx::Size& size) {
  Send(new ViewMsg_DisableScrollbarsForSmallWindows(GetRoutingID(), size));
}
