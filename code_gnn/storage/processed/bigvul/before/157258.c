void WebMediaPlayerImpl::OnFFmpegMediaTracksUpdated(
    std::unique_ptr<MediaTracks> tracks) {
  DCHECK(demuxer_.get());
  DCHECK(!chunk_demuxer_);

  bool is_first_audio_track = true;
  bool is_first_video_track = true;
  for (const auto& track : tracks->tracks()) {
    if (track->type() == MediaTrack::Audio) {
      client_->AddAudioTrack(blink::WebString::FromUTF8(track->id()),
                             blink::WebMediaPlayerClient::kAudioTrackKindMain,
                             blink::WebString::FromUTF8(track->label()),
                             blink::WebString::FromUTF8(track->language()),
                             is_first_audio_track);
      is_first_audio_track = false;
    } else if (track->type() == MediaTrack::Video) {
      client_->AddVideoTrack(blink::WebString::FromUTF8(track->id()),
                             blink::WebMediaPlayerClient::kVideoTrackKindMain,
                             blink::WebString::FromUTF8(track->label()),
                             blink::WebString::FromUTF8(track->language()),
                             is_first_video_track);
      is_first_video_track = false;
    } else {
      NOTREACHED();
    }
  }
}
