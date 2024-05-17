void TabLoader::OnOnlineStateChanged(bool online) {
  if (online) {
    if (!loading_) {
      loading_ = true;
      LoadNextTab();
    }
  } else {
    loading_ = false;
  }
}
