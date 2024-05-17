WebMediaPlayer::TrackId HTMLMediaElement::AddAudioTrack(
    const WebString& id,
    WebMediaPlayerClient::AudioTrackKind kind,
    const WebString& label,
    const WebString& language,
    bool enabled) {
  AtomicString kind_string = AudioKindToString(kind);
  BLINK_MEDIA_LOG << "addAudioTrack(" << (void*)this << ", '" << (String)id
                  << "', ' " << (AtomicString)kind_string << "', '"
                  << (String)label << "', '" << (String)language << "', "
                  << BoolString(enabled) << ")";

  AudioTrack* audio_track =
      AudioTrack::Create(id, kind_string, label, language, enabled);
  audioTracks().Add(audio_track);

  return audio_track->id();
}
