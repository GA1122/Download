void RenderFrameImpl::CheckIfAudioSinkExistsAndIsAuthorized(
    const blink::WebString& sink_id,
    std::unique_ptr<blink::WebSetSinkIdCallbacks> callbacks) {
  std::move(media::ConvertToOutputDeviceStatusCB(std::move(callbacks)))
      .Run(AudioDeviceFactory::GetOutputDeviceInfo(
               GetRoutingID(), media::AudioSinkParameters(0, sink_id.Utf8()))
               .device_status());
}
