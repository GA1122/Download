bool MediaRecorderHandler::Initialize(
    blink::WebMediaRecorderHandlerClient* client,
    const blink::WebMediaStream& media_stream,
    const blink::WebString& type,
    const blink::WebString& codecs,
    int32_t audio_bits_per_second,
    int32_t video_bits_per_second) {
  DCHECK(main_render_thread_checker_.CalledOnValidThread());
  UpdateWebRTCMethodCount(blink::WebRTCAPIName::kMediaStreamRecorder);

  if (!CanSupportMimeType(type, codecs)) {
    DLOG(ERROR) << "Unsupported " << type.Utf8() << ";codecs=" << codecs.Utf8();
    return false;
  }

  const VideoTrackRecorder::CodecId video_codec_id =
      VideoStringToCodecId(codecs);
  video_codec_id_ = (video_codec_id != VideoTrackRecorder::CodecId::LAST)
                        ? video_codec_id
                        : VideoTrackRecorder::GetPreferredCodecId();
  DVLOG_IF(1, video_codec_id == VideoTrackRecorder::CodecId::LAST)
      << "Falling back to preferred video codec id "
      << static_cast<int>(video_codec_id_);

  const AudioTrackRecorder::CodecId audio_codec_id =
      AudioStringToCodecId(codecs);
  audio_codec_id_ = (audio_codec_id != AudioTrackRecorder::CodecId::LAST)
                        ? audio_codec_id
                        : AudioTrackRecorder::GetPreferredCodecId();
  DVLOG_IF(1, audio_codec_id == AudioTrackRecorder::CodecId::LAST)
      << "Falling back to preferred audio codec id "
      << static_cast<int>(audio_codec_id_);

  media_stream_ = media_stream;
  DCHECK(client);
  client_ = client;

  audio_bits_per_second_ = audio_bits_per_second;
  video_bits_per_second_ = video_bits_per_second;
  return true;
}
