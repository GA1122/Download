void ProfileSyncService::RefreshSpareBootstrapToken(
    const std::string& passphrase) {
  browser_sync::ChromeEncryptor encryptor;
  browser_sync::Cryptographer temp_cryptographer(&encryptor);
  browser_sync::KeyParams key_params = {"localhost", "dummy", passphrase};

  std::string bootstrap_token;
  if (!temp_cryptographer.AddKey(key_params)) {
    NOTREACHED() << "Failed to add key to cryptographer.";
  }
  temp_cryptographer.GetBootstrapToken(&bootstrap_token);
  sync_prefs_.SetSpareBootstrapToken(bootstrap_token);
}
