bool Browser::RunUnloadEventsHelper(TabContents* contents) {
  if (contents->NeedToFireBeforeUnload()) {
    contents->render_view_host()->FirePageBeforeUnload(false);
    return true;
  }
  return false;
}
