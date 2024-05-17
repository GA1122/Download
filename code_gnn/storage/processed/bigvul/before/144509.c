WebContents* WebContents::FromRenderViewHost(RenderViewHost* rvh) {
  if (!rvh)
    return nullptr;
  return rvh->GetDelegate()->GetAsWebContents();
}
