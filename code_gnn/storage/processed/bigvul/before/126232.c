bool Browser::RunUnloadEventsHelper(WebContents* contents) {
  if (contents->NeedToFireBeforeUnload()) {
    contents->GetRenderViewHost()->FirePageBeforeUnload(false);
    return true;
  }
  return false;
}
