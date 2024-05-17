void WebstoreStandaloneInstaller::CompleteInstall(
    webstore_install::Result result,
    const std::string& error) {
  scoped_active_install_.reset();
  if (!callback_.is_null())
    callback_.Run(result == webstore_install::SUCCESS, error, result);
  Release();   
}
