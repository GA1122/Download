MediaStreamDispatcher* PepperMediaDeviceManager::GetMediaStreamDispatcher()
    const {
  DCHECK(render_frame());
  MediaStreamDispatcher* const dispatcher =
      static_cast<RenderFrameImpl*>(render_frame())->GetMediaStreamDispatcher();
  DCHECK(dispatcher);
  return dispatcher;
}
