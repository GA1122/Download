bool Browser::RunUnloadListenerBeforeClosing(TabContentsWrapper* contents) {
  return Browser::RunUnloadEventsHelper(contents->tab_contents());
}
