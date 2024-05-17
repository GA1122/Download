Optional<WebAddressSpace> WorkerFetchContext::GetAddressSpace() const {
  return WTF::make_optional(global_scope_->GetSecurityContext().AddressSpace());
}
