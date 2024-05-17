void GaiaCookieManagerService::ExternalCcResultFetcher::Timeout() {
  CleanupTransientState();
  GetCheckConnectionInfoCompleted(false);
}
