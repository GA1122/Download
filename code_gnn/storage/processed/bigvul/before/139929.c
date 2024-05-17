WebMediaPlayer::TrackId HTMLMediaElement::addVideoTrack(
    const WebString& id,
    WebMediaPlayerClient::VideoTrackKind kind,
    const WebString& label,
    const WebString& language,
    bool selected) {
  AtomicString kindString = VideoKindToString(kind);
  BLINK_MEDIA_LOG << "addVideoTrack(" << (void*)this << ", '" << (String)id
                  << "', '" << (AtomicString)kindString << "', '"
                  << (String)label << "', '" << (String)language << "', "
                  << boolString(selected) << ")";

  if (selected && videoTracks().selectedIndex() != -1)
    selected = false;

  VideoTrack* videoTrack =
      VideoTrack::create(id, kindString, label, language, selected);
  videoTracks().add(videoTrack);

  return videoTrack->id();
}
