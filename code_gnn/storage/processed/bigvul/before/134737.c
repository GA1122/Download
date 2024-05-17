void GuestViewBase::LoadingStateChanged(content::WebContents* source,
                                        bool to_different_document) {
  if (!attached() || !embedder_web_contents()->GetDelegate())
    return;

  embedder_web_contents()->GetDelegate()->LoadingStateChanged(
      embedder_web_contents(), to_different_document);
}
