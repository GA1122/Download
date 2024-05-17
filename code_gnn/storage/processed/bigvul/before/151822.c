void BackgroundLoaderOfflinerTest::OnProgress(const SavePageRequest& request,
                                              int64_t bytes) {
  progress_ = bytes;
}
