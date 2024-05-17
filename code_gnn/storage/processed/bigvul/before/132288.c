blink::WebServiceWorkerProvider* RenderFrameImpl::createServiceWorkerProvider(
    blink::WebLocalFrame* frame) {
  DCHECK(!frame_ || frame_ == frame);
  DCHECK(frame->dataSource());
  if (!ChildThreadImpl::current())
    return NULL;   
  ServiceWorkerNetworkProvider* provider =
      ServiceWorkerNetworkProvider::FromDocumentState(
          DocumentState::FromDataSource(frame->dataSource()));
  DCHECK(provider);
  return new WebServiceWorkerProviderImpl(
      ChildThreadImpl::current()->thread_safe_sender(),
      provider->context());
}
