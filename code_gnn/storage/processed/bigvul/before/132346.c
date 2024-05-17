 blink::WebVRClient* RenderFrameImpl::webVRClient() {
   if (!vr_dispatcher_)
    vr_dispatcher_.reset(new VRDispatcher(GetServiceRegistry()));

  return vr_dispatcher_.get();
}
