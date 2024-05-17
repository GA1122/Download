bool Document::TasksNeedSuspension() {
  Page* page = GetPage();
  return page && page->Paused();
}
