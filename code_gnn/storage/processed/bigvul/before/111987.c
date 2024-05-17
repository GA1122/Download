Browser* SyncTest::GetBrowser(int index) {
  if (browsers_.empty())
    LOG(FATAL) << "SetupClients() has not yet been called.";
  if (index < 0 || index >= static_cast<int>(browsers_.size()))
    LOG(FATAL) << "GetBrowser(): Index is out of bounds.";
  return browsers_[index];
}
