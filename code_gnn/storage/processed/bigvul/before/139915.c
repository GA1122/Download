const AtomicString& AudioKindToString(
    WebMediaPlayerClient::AudioTrackKind kind) {
  switch (kind) {
    case WebMediaPlayerClient::AudioTrackKindNone:
      return emptyAtom;
    case WebMediaPlayerClient::AudioTrackKindAlternative:
      return AudioTrack::alternativeKeyword();
    case WebMediaPlayerClient::AudioTrackKindDescriptions:
      return AudioTrack::descriptionsKeyword();
    case WebMediaPlayerClient::AudioTrackKindMain:
      return AudioTrack::mainKeyword();
    case WebMediaPlayerClient::AudioTrackKindMainDescriptions:
      return AudioTrack::mainDescriptionsKeyword();
    case WebMediaPlayerClient::AudioTrackKindTranslation:
      return AudioTrack::translationKeyword();
    case WebMediaPlayerClient::AudioTrackKindCommentary:
      return AudioTrack::commentaryKeyword();
  }

  NOTREACHED();
  return emptyAtom;
}
