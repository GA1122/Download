Optional<WebAddressSpace> FrameFetchContext::GetAddressSpace() const {
  if (IsDetached())
    return frozen_state_->address_space;
  if (!document_)
    return WTF::nullopt;
  ExecutionContext* context = document_;
  return WTF::make_optional(context->GetSecurityContext().AddressSpace());
}
