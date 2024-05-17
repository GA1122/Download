void GuestViewBase::UpdateTargetURL(content::WebContents* source,
                                    const GURL& url) {
  if (!attached() || !embedder_web_contents()->GetDelegate())
    return;

  embedder_web_contents()->GetDelegate()->UpdateTargetURL(
      embedder_web_contents(), url);
}
