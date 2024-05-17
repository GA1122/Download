void RunGetCallback(GetCallback callback, const CredentialInfo& info) {
  std::move(callback).Run(CredentialManagerError::SUCCESS, info);
}
