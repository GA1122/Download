PassOwnPtr<WebPluginLoadObserver> WebFrameLoaderClient::GetPluginLoadObserver() {
  WebDataSourceImpl* ds = WebDataSourceImpl::fromDocumentLoader(
      webframe_->frame()->loader()->activeDocumentLoader());
  return ds->releasePluginLoadObserver();
}
