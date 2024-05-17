GURL RenderViewImpl::GetLoadingUrl(WebKit::WebFrame* frame) const {
  WebDataSource* ds = frame->dataSource();
  if (ds->hasUnreachableURL())
    return ds->unreachableURL();

  const WebURLRequest& request = ds->request();
  return request.url();
}
