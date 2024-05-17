content::RenderFrameHost* GetMostVisitedIframe(content::WebContents* tab) {
  CHECK_EQ(2u, tab->GetAllFrames().size());
  for (content::RenderFrameHost* frame : tab->GetAllFrames()) {
    if (frame != tab->GetMainFrame()) {
      return frame;
    }
  }
  NOTREACHED();
  return nullptr;
}
