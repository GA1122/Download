void AddDataReductionProxyBinding(
    content::ResourceContext* resource_context,
    data_reduction_proxy::mojom::DataReductionProxyRequest request) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  auto* io_data = ProfileIOData::FromResourceContext(resource_context);
  if (io_data && io_data->data_reduction_proxy_io_data()) {
    io_data->data_reduction_proxy_io_data()->Clone(std::move(request));
  }
}
