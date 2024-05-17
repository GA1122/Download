void VirtualAuthenticator::ClearRegistrations(
    ClearRegistrationsCallback callback) {
  state_->registrations.clear();
  std::move(callback).Run();
}
