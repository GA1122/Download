void RenderFrameImpl::CheckIfAudioSinkExistsAndIsAuthorized(
    const blink::WebString& sink_id,
    blink::WebSetSinkIdCompleteCallback completion_callback) {
  std::move(
      media::ConvertToOutputDeviceStatusCB(std::move(completion_callback)))
      .Run(AudioDeviceFactory::GetOutputDeviceInfo(
               GetRoutingID(), media::AudioSinkParameters(0, sink_id.Utf8()))
               .device_status());
}
