bool WebContentsImpl::ContainsOrIsFocusedWebContents() {
  for (WebContentsImpl* focused_contents = GetFocusedWebContents();
       focused_contents;
       focused_contents = focused_contents->GetOuterWebContents()) {
    if (focused_contents == this)
      return true;
  }

  return false;
}
