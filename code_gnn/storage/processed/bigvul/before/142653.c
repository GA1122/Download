void WebstoreStandaloneInstaller::RunCallback(bool success,
                                              const std::string& error,
                                              webstore_install::Result result) {
  callback_.Run(success, error, result);
}
