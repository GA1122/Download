void Bluetooth::RequestScanningCallback(
    ScriptPromiseResolver* resolver,
    mojo::BindingId id,
    mojom::blink::RequestScanningStartResultPtr result) {
  if (!resolver->GetExecutionContext() ||
      resolver->GetExecutionContext()->IsContextDestroyed()) {
    return;
  }

  if (result->is_error_result()) {
    resolver->Reject(
        BluetoothError::CreateDOMException(result->get_error_result()));
    return;
  }

  auto* scan = MakeGarbageCollected<BluetoothLEScan>(
      id, this, std::move(result->get_options()));
  resolver->Resolve(scan);
}
