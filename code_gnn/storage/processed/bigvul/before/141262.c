bool Document::TasksNeedPause() {
  Page* page = GetPage();
  return page && page->Paused();
}
