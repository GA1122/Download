content::RenderFrameHost* GetMainFrame(content::RenderFrameHost* rfh) {
  while (rfh->GetParent() != nullptr)
    rfh = rfh->GetParent();
  return rfh;
}
