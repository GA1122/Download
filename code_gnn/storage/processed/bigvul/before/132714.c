void ChromotingInstance::FetchSecretFromString(
    const std::string& shared_secret,
    bool pairing_supported,
    const protocol::SecretFetchedCallback& secret_fetched_callback) {
  secret_fetched_callback.Run(shared_secret);
}
