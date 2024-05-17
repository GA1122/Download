void SearchProviderTest::OnProviderUpdate(bool updated_matches) {
  if (quit_when_done_ && provider_->done()) {
    quit_when_done_ = false;
    message_loop_.Quit();
  }
}
