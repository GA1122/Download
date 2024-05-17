void WebstoreStandaloneInstaller::OnWebstoreResponseParseFailure(
    const std::string& error) {
  OnWebStoreDataFetcherDone();
  CompleteInstall(webstore_install::INVALID_WEBSTORE_RESPONSE, error);
}
