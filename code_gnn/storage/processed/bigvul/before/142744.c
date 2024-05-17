const AtomicString& AudioKindToString(
    WebMediaPlayerClient::AudioTrackKind kind) {
  switch (kind) {
    case WebMediaPlayerClient::kAudioTrackKindNone:
      return g_empty_atom;
    case WebMediaPlayerClient::kAudioTrackKindAlternative:
      return AudioTrack::AlternativeKeyword();
    case WebMediaPlayerClient::kAudioTrackKindDescriptions:
      return AudioTrack::DescriptionsKeyword();
    case WebMediaPlayerClient::kAudioTrackKindMain:
      return AudioTrack::MainKeyword();
    case WebMediaPlayerClient::kAudioTrackKindMainDescriptions:
      return AudioTrack::MainDescriptionsKeyword();
    case WebMediaPlayerClient::kAudioTrackKindTranslation:
      return AudioTrack::TranslationKeyword();
    case WebMediaPlayerClient::kAudioTrackKindCommentary:
      return AudioTrack::CommentaryKeyword();
  }

  NOTREACHED();
  return g_empty_atom;
}
