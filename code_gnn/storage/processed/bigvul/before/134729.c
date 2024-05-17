void GuestViewBase::HandleKeyboardEvent(
    WebContents* source,
    const content::NativeWebKeyboardEvent& event) {
  if (!attached())
    return;

  embedder_web_contents()->GetDelegate()->
      HandleKeyboardEvent(embedder_web_contents(), event);
}
