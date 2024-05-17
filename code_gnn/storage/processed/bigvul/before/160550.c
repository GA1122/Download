void RenderFrameImpl::CheckIfAudioSinkExistsAndIsAuthorized(
    const blink::WebString& sink_id,
    const blink::WebSecurityOrigin& security_origin,
    blink::WebSetSinkIdCallbacks* web_callbacks) {
  media::OutputDeviceStatusCB callback =
      media::ConvertToOutputDeviceStatusCB(web_callbacks);
  callback.Run(AudioDeviceFactory::GetOutputDeviceInfo(
                   GetRoutingID(), 0, sink_id.Utf8(), security_origin)
                   .device_status());
}
