void GuestViewBase::ContentsMouseEvent(content::WebContents* source,
                                       const gfx::Point& location,
                                       bool motion) {
  if (!attached() || !embedder_web_contents()->GetDelegate())
    return;

  embedder_web_contents()->GetDelegate()->ContentsMouseEvent(
      embedder_web_contents(), location, motion);
}
