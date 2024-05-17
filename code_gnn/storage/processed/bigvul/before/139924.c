WebMediaPlayer::TrackId HTMLMediaElement::addAudioTrack(
    const WebString& id,
    WebMediaPlayerClient::AudioTrackKind kind,
    const WebString& label,
    const WebString& language,
    bool enabled) {
  AtomicString kindString = AudioKindToString(kind);
  BLINK_MEDIA_LOG << "addAudioTrack(" << (void*)this << ", '" << (String)id
                  << "', ' " << (AtomicString)kindString << "', '"
                  << (String)label << "', '" << (String)language << "', "
                  << boolString(enabled) << ")";

  AudioTrack* audioTrack =
      AudioTrack::create(id, kindString, label, language, enabled);
  audioTracks().add(audioTrack);

  return audioTrack->id();
}
