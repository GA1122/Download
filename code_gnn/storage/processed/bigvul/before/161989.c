void PdfCompositorClient::Connect(service_manager::Connector* connector) {
  DCHECK(!compositor_.is_bound());
  connector->BindInterface(mojom::kServiceName, &compositor_);
}
