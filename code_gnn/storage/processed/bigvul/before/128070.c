scoped_ptr<SynchronousCompositorImpl> SynchronousCompositorImpl::Create(
    RenderWidgetHostViewAndroid* rwhva,
    WebContents* web_contents) {
  DCHECK(web_contents);
  WebContentsAndroid* web_contents_android =
      static_cast<WebContentsImpl*>(web_contents)->GetWebContentsAndroid();
  if (!web_contents_android->synchronous_compositor_client())
    return nullptr;   

  return make_scoped_ptr(new SynchronousCompositorImpl(
      rwhva, web_contents_android->synchronous_compositor_client()));
}
