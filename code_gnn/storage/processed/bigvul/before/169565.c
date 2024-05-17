void CastStreamingNativeHandler::AddTracksToMediaStream(
    const std::string& url,
    const media::AudioParameters& params,
    scoped_refptr<media::AudioCapturerSource> audio,
    scoped_ptr<media::VideoCapturerSource> video) {
  blink::WebMediaStream web_stream =
      blink::WebMediaStreamRegistry::lookupMediaStreamDescriptor(GURL(url));
  if (web_stream.isNull()) {
    LOG(DFATAL) << "Stream not found.";
    return;
  }
  if (!content::AddAudioTrackToMediaStream(
          audio, params.sample_rate(), params.channel_layout(),
          params.frames_per_buffer(), true,   
          true,                               
          &web_stream)) {
    LOG(ERROR) << "Failed to add Cast audio track to media stream.";
  }
  if (!content::AddVideoTrackToMediaStream(std::move(video), true,   
                                           true,   
                                           &web_stream)) {
    LOG(ERROR) << "Failed to add Cast video track to media stream.";
  }
}
