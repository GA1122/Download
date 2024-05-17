bool Document::TasksNeedSuspension() {
  Page* page = this->GetPage();
  return page && page->Paused();
}
