Profile* LiveSyncTest::GetProfile(int index) {
  if (profiles_.empty())
    LOG(FATAL) << "SetupSync() has not yet been called.";
  if (index < 0 || index >= static_cast<int>(profiles_.size()))
    LOG(FATAL) << "GetProfile(): Index is out of bounds.";
  return profiles_[index];
}