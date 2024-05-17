std::unique_ptr<::device::FidoDevice> VirtualAuthenticator::ConstructDevice() {
  return std::make_unique<::device::VirtualU2fDevice>(state_);
}
