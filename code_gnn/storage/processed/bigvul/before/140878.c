blink::mojom::PresentationConnectionPtr ControllerConnectionProxy::Bind() {
  return binding_.CreateInterfacePtrAndBind();
}
