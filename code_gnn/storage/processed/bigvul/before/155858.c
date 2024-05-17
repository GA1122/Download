void SupervisedUserService::SetSafeSearchURLReporter(
    std::unique_ptr<SafeSearchURLReporter> reporter) {
  url_reporter_ = std::move(reporter);
}
