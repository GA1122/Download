base::Optional<mojom::IPAddressSpace> FrameFetchContext::GetAddressSpace()
    const {
  if (IsDetached())
    return frozen_state_->address_space;
  if (!document_)
    return base::nullopt;
  ExecutionContext* context = document_;
  return base::make_optional(context->GetSecurityContext().AddressSpace());
}
