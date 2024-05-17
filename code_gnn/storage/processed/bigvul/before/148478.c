void WebContentsImpl::OnFindMatchRectsReply(
    RenderFrameHostImpl* source,
    int version,
    const std::vector<gfx::RectF>& rects,
    const gfx::RectF& active_rect) {
  GetOrCreateFindRequestManager()->OnFindMatchRectsReply(source, version, rects,
                                                         active_rect);
}
