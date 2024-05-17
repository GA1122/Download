void WebContentsImpl::AttachToOuterWebContentsFrame(
    WebContents* outer_web_contents,
    RenderFrameHost* outer_contents_frame) {
  CHECK(BrowserPluginGuestMode::UseCrossProcessFramesForGuests());
  node_.reset(new WebContentsTreeNode());
  node_->ConnectToOuterWebContents(
      static_cast<WebContentsImpl*>(outer_web_contents),
      static_cast<RenderFrameHostImpl*>(outer_contents_frame));

  DCHECK(outer_contents_frame);

  GetRenderManager()->CreateOuterDelegateProxy(
      outer_contents_frame->GetSiteInstance(),
      static_cast<RenderFrameHostImpl*>(outer_contents_frame));

  GetRenderManager()->SetRWHViewForInnerContents(
      GetRenderManager()->GetRenderWidgetHostView());
}
