void GuestViewBase::DidStopLoading() {
  content::RenderViewHost* rvh = web_contents()->GetRenderViewHost();

  if (IsPreferredSizeModeEnabled())
    rvh->EnablePreferredSizeMode();
  if (!IsDragAndDropEnabled()) {
    const char script[] =
        "window.addEventListener('dragstart', function() { "
        "  window.event.preventDefault(); "
        "});";
    rvh->GetMainFrame()->ExecuteJavaScript(base::ASCIIToUTF16(script));
  }
  GuestViewDidStopLoading();
}
