void ForwardShapeDetectionRequest(R request) {
  service_manager::Connector* connector =
      ServiceManagerConnection::GetForProcess()->GetConnector();
  connector->BindInterface(shape_detection::mojom::kServiceName,
                           std::move(request));
}
