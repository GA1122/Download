RenderFrameImpl::CreateRendererFactory() {
  scoped_ptr<MediaStreamRendererFactory> factory =
      GetContentClient()->renderer()->CreateMediaStreamRendererFactory();
  if (factory.get())
    return factory.Pass();
#if defined(ENABLE_WEBRTC)
  return scoped_ptr<MediaStreamRendererFactory>(
      new MediaStreamRendererFactoryImpl());
#else
  return scoped_ptr<MediaStreamRendererFactory>(
      static_cast<MediaStreamRendererFactory*>(NULL));
#endif
}
