bool MediaRecorderHandler::Start(int timeslice) {
  DCHECK(main_render_thread_checker_.CalledOnValidThread());
  DCHECK(!recording_);
  DCHECK(!media_stream_.IsNull());
  DCHECK(timeslice_.is_zero());
  DCHECK(!webm_muxer_);

  timeslice_ = TimeDelta::FromMilliseconds(timeslice);
  slice_origin_timestamp_ = TimeTicks::Now();

  video_tracks_ = media_stream_.VideoTracks();
  audio_tracks_ = media_stream_.AudioTracks();

  if (video_tracks_.IsEmpty() && audio_tracks_.IsEmpty()) {
    LOG(WARNING) << __func__ << ": no media tracks.";
    return false;
  }

  const bool use_video_tracks =
      !video_tracks_.IsEmpty() && video_tracks_[0].IsEnabled() &&
      video_tracks_[0].Source().GetReadyState() !=
          blink::WebMediaStreamSource::kReadyStateEnded;
  const bool use_audio_tracks =
      !audio_tracks_.IsEmpty() &&
      MediaStreamAudioTrack::From(audio_tracks_[0]) &&
      audio_tracks_[0].IsEnabled() &&
      audio_tracks_[0].Source().GetReadyState() !=
          blink::WebMediaStreamSource::kReadyStateEnded;

  if (!use_video_tracks && !use_audio_tracks) {
    LOG(WARNING) << __func__ << ": no tracks to be recorded.";
    return false;
  }

  webm_muxer_.reset(
      new media::WebmMuxer(CodecIdToMediaVideoCodec(video_codec_id_),
                           CodecIdToMediaAudioCodec(audio_codec_id_),
                           use_video_tracks, use_audio_tracks,
                           base::Bind(&MediaRecorderHandler::WriteData,
                                      weak_factory_.GetWeakPtr())));

  if (use_video_tracks) {
    LOG_IF(WARNING, video_tracks_.size() > 1u)
        << "Recording multiple video tracks is not implemented. "
        << "Only recording first video track.";
    const blink::WebMediaStreamTrack& video_track = video_tracks_[0];
    if (video_track.IsNull())
      return false;

    const VideoTrackRecorder::OnEncodedVideoCB on_encoded_video_cb =
        media::BindToCurrentLoop(base::Bind(
            &MediaRecorderHandler::OnEncodedVideo, weak_factory_.GetWeakPtr()));

    video_recorders_.emplace_back(new VideoTrackRecorder(
        video_codec_id_, video_track, on_encoded_video_cb,
        video_bits_per_second_, task_runner_));
  }

  if (use_audio_tracks) {
    LOG_IF(WARNING, audio_tracks_.size() > 1u)
        << "Recording multiple audio"
        << " tracks is not implemented.  Only recording first audio track.";
    const blink::WebMediaStreamTrack& audio_track = audio_tracks_[0];
    if (audio_track.IsNull())
      return false;

    const AudioTrackRecorder::OnEncodedAudioCB on_encoded_audio_cb =
        media::BindToCurrentLoop(base::Bind(
            &MediaRecorderHandler::OnEncodedAudio, weak_factory_.GetWeakPtr()));

    audio_recorders_.emplace_back(new AudioTrackRecorder(
        audio_codec_id_, audio_track, std::move(on_encoded_audio_cb),
        audio_bits_per_second_));
  }

  recording_ = true;
  return true;
}
