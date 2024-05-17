void RenderFrameHostImpl::BindAuthenticatorRequest(
    webauth::mojom::AuthenticatorRequest request) {
  if (!authenticator_impl_)
    authenticator_impl_.reset(new AuthenticatorImpl(this));

  authenticator_impl_->Bind(std::move(request));
}
