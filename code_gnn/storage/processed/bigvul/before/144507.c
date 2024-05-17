WebContents* WebContents::FromRenderFrameHost(RenderFrameHost* rfh) {
  if (!rfh)
    return nullptr;
  return static_cast<RenderFrameHostImpl*>(rfh)->delegate()->GetAsWebContents();
}
