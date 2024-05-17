void ParseStreamType(const StreamControls& controls,
                     MediaStreamType* audio_type,
                     MediaStreamType* video_type) {
  *audio_type = MEDIA_NO_SERVICE;
  *video_type = MEDIA_NO_SERVICE;
  const bool audio_support_flag_for_desktop_share =
      !base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDisableAudioSupportForDesktopShare);
  if (controls.audio.requested) {
    if (!controls.audio.stream_source.empty()) {
       if (controls.audio.stream_source == kMediaStreamSourceTab) {
         *audio_type = MEDIA_TAB_AUDIO_CAPTURE;
       } else if (controls.audio.stream_source == kMediaStreamSourceSystem) {
         *audio_type = MEDIA_DESKTOP_AUDIO_CAPTURE;
       } else if (audio_support_flag_for_desktop_share &&
                  controls.audio.stream_source == kMediaStreamSourceDesktop) {
         *audio_type = MEDIA_DESKTOP_AUDIO_CAPTURE;
       }
     } else {
       *audio_type = MEDIA_DEVICE_AUDIO_CAPTURE;
     }
  }
  if (controls.video.requested) {
    if (!controls.video.stream_source.empty()) {
      if (controls.video.stream_source == kMediaStreamSourceTab) {
        *video_type = MEDIA_TAB_VIDEO_CAPTURE;
      } else if (controls.video.stream_source == kMediaStreamSourceScreen) {
        *video_type = MEDIA_DESKTOP_VIDEO_CAPTURE;
      } else if (controls.video.stream_source == kMediaStreamSourceDesktop) {
        *video_type = MEDIA_DESKTOP_VIDEO_CAPTURE;
      }
    } else {
      *video_type = MEDIA_DEVICE_VIDEO_CAPTURE;
    }
  }
}
