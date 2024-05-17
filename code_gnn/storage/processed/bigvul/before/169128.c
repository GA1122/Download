void AddFrameToSet(std::set<content::RenderFrameHost*>* frames,
                   content::RenderFrameHost* rfh) {
  if (rfh->IsRenderFrameLive())
    frames->insert(rfh);
}
