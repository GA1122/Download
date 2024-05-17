void RenderViewImpl::didAdoptURLLoader(WebKit::WebURLLoader* loader) {
  webkit_glue::WebURLLoaderImpl* loader_impl =
      static_cast<webkit_glue::WebURLLoaderImpl*>(loader);
  loader_impl->UpdateRoutingId(routing_id_);
}
