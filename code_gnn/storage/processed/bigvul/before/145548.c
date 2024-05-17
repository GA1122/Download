void VirtualAuthenticator::GetUserPresence(GetUserPresenceCallback callback) {
  std::move(callback).Run(false);
}
