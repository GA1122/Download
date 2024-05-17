void VirtualAuthenticator::SetUserPresence(bool present,
                                           SetUserPresenceCallback callback) {
  std::move(callback).Run();
}
