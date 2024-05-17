PassRefPtr<DocumentLoader> WebFrameLoaderClient::createDocumentLoader(
    const ResourceRequest& request,
    const SubstituteData& data) {
  RefPtr<WebDataSourceImpl> ds = WebDataSourceImpl::create(request, data);
  if (webframe_->client())
    webframe_->client()->didCreateDataSource(webframe_, ds.get());
  return ds.release();
}
