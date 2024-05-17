void Document::SendDidEditFieldInInsecureContext() {
  if (!GetFrame())
    return;

  mojom::blink::InsecureInputServicePtr insecure_input_service_ptr;
  GetFrame()->GetInterfaceProvider().GetInterface(
      mojo::MakeRequest(&insecure_input_service_ptr));

  insecure_input_service_ptr->DidEditFieldInInsecureContext();
}
