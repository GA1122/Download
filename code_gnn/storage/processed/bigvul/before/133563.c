WebContents* WebContents::FromRenderFrameHost(RenderFrameHost* rfh) {
  RenderFrameHostImpl* rfh_impl = static_cast<RenderFrameHostImpl*>(rfh);
  if (!rfh_impl)
    return NULL;
  return rfh_impl->delegate()->GetAsWebContents();
}
