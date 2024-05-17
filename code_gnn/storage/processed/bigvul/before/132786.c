 void PepperVideoRenderer3D::OnSessionConfig(
     const protocol::SessionConfig& config) {
   PP_VideoProfile video_profile = PP_VIDEOPROFILE_VP8_ANY;
  switch (config.video_config().codec) {
    case protocol::ChannelConfig::CODEC_VP8:
      video_profile = PP_VIDEOPROFILE_VP8_ANY;
      break;
    case protocol::ChannelConfig::CODEC_VP9:
      video_profile = PP_VIDEOPROFILE_VP9_ANY;
      break;
    default:
      NOTREACHED();
  }
  int32_t result = video_decoder_.Initialize(
      graphics_, video_profile, PP_HARDWAREACCELERATION_WITHFALLBACK,
      callback_factory_.NewCallback(&PepperVideoRenderer3D::OnInitialized));
  CHECK_EQ(result, PP_OK_COMPLETIONPENDING)
      << "video_decoder_.Initialize() returned " << result;
}
