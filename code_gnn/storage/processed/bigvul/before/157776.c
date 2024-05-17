device::mojom::GeolocationContext* WebContentsImpl::GetGeolocationContext() {
  if (geolocation_context_)
    return geolocation_context_.get();

  auto request = mojo::MakeRequest(&geolocation_context_);
  if (!ServiceManagerConnection::GetForProcess())
    return geolocation_context_.get();

  service_manager::Connector* connector =
      ServiceManagerConnection::GetForProcess()->GetConnector();
  connector->BindInterface(device::mojom::kServiceName, std::move(request));
  return geolocation_context_.get();
}
