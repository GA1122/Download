void GuestViewBase::DeactivateContents(WebContents* web_contents) {
  if (!attached() || !embedder_web_contents()->GetDelegate())
    return;

  embedder_web_contents()->GetDelegate()->DeactivateContents(
      embedder_web_contents());
}
