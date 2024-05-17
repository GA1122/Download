void MediaRecorderHandler::EncodingInfo(
    const blink::WebMediaConfiguration& configuration,
    std::unique_ptr<blink::WebMediaCapabilitiesQueryCallbacks> callbacks) {
  DCHECK(main_render_thread_checker_.CalledOnValidThread());
  DCHECK(configuration.video_configuration ||
         configuration.audio_configuration);

  auto scoped_callbacks = blink::MakeScopedWebCallbacks(
      std::move(callbacks), base::BindOnce(&OnEncodingInfoError));

  std::unique_ptr<blink::WebMediaCapabilitiesInfo> info(
      new blink::WebMediaCapabilitiesInfo());

  blink::WebString mime_type;
  blink::WebString codec;
  if (configuration.video_configuration) {
    mime_type = configuration.video_configuration->mime_type;
    codec = configuration.video_configuration->codec;
  } else {
    mime_type = configuration.audio_configuration->mime_type;
    codec = configuration.audio_configuration->codec;
  }

  info->supported = CanSupportMimeType(mime_type, codec);

  if (configuration.video_configuration && info->supported) {
    const bool is_likely_accelerated =
        VideoTrackRecorder::CanUseAcceleratedEncoder(
            VideoStringToCodecId(codec),
            configuration.video_configuration->width,
            configuration.video_configuration->height);

    const float pixels_per_second =
        configuration.video_configuration->width *
        configuration.video_configuration->height *
        configuration.video_configuration->framerate;
    const float threshold = base::SysInfo::IsLowEndDevice()
                                ? kNumPixelsPerSecondSmoothnessThresholdLow
                                : kNumPixelsPerSecondSmoothnessThresholdHigh;
    info->smooth = is_likely_accelerated || pixels_per_second <= threshold;

    info->power_efficient = info->smooth;
  }
  DVLOG(1) << "type: " << mime_type.Ascii() << ", params:" << codec.Ascii()
           << " is" << (info->supported ? " supported" : " NOT supported")
           << " and" << (info->smooth ? " smooth" : " NOT smooth");

  scoped_callbacks.PassCallbacks()->OnSuccess(std::move(info));
}
