blink::WebString MediaRecorderHandler::ActualMimeType() {
  DCHECK(main_render_thread_checker_.CalledOnValidThread());
  DCHECK(client_) << __func__ << " should be called after Initialize()";

  const bool has_video_tracks = !media_stream_.VideoTracks().empty();
  const bool has_audio_tracks = !media_stream_.AudioTracks().empty();
  if (!has_video_tracks && !has_audio_tracks)
    return blink::WebString();

  std::string mime_type;
  if (!has_video_tracks && has_audio_tracks) {
    mime_type.append("audio/webm;codecs=");
  } else {
    switch (video_codec_id_) {
      case VideoTrackRecorder::CodecId::VP8:
      case VideoTrackRecorder::CodecId::VP9:
        mime_type.append("video/webm;codecs=");
        break;
#if BUILDFLAG(RTC_USE_H264)
      case VideoTrackRecorder::CodecId::H264:
        mime_type.append("video/x-matroska;codecs=");
        break;
#endif
      case VideoTrackRecorder::CodecId::LAST:
        break;
    }
  }
  if (has_video_tracks) {
    switch (video_codec_id_) {
      case VideoTrackRecorder::CodecId::VP8:
        mime_type.append("vp8");
        break;
      case VideoTrackRecorder::CodecId::VP9:
        mime_type.append("vp9");
        break;
#if BUILDFLAG(RTC_USE_H264)
      case VideoTrackRecorder::CodecId::H264:
        mime_type.append("avc1");
        break;
#endif
      case VideoTrackRecorder::CodecId::LAST:
        DCHECK_NE(audio_codec_id_, AudioTrackRecorder::CodecId::LAST);
    }
  }
  if (has_video_tracks && has_audio_tracks) {
    if (video_codec_id_ != VideoTrackRecorder::CodecId::LAST &&
        audio_codec_id_ != AudioTrackRecorder::CodecId::LAST) {
      mime_type.append(",");
    }
  }
  if (has_audio_tracks) {
    switch (audio_codec_id_) {
      case AudioTrackRecorder::CodecId::OPUS:
        mime_type.append("opus");
        break;
      case AudioTrackRecorder::CodecId::PCM:
        mime_type.append("pcm");
        break;
      case AudioTrackRecorder::CodecId::LAST:
        DCHECK_NE(video_codec_id_, VideoTrackRecorder::CodecId::LAST);
    }
  }
  return blink::WebString::FromUTF8(mime_type);
}
