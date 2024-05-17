void VirtualAuthenticator::AddBinding(
    blink::test::mojom::VirtualAuthenticatorRequest request) {
  binding_set_.AddBinding(this, std::move(request));
}
