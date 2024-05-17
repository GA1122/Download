bool Browser::ShouldStartShutdown() const {
  return BrowserList::GetInstance()->size() <= 1;
}
