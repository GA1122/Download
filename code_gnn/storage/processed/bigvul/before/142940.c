const AtomicString& VideoKindToString(
    WebMediaPlayerClient::VideoTrackKind kind) {
  switch (kind) {
    case WebMediaPlayerClient::kVideoTrackKindNone:
      return g_empty_atom;
    case WebMediaPlayerClient::kVideoTrackKindAlternative:
      return VideoTrack::AlternativeKeyword();
    case WebMediaPlayerClient::kVideoTrackKindCaptions:
      return VideoTrack::CaptionsKeyword();
    case WebMediaPlayerClient::kVideoTrackKindMain:
      return VideoTrack::MainKeyword();
    case WebMediaPlayerClient::kVideoTrackKindSign:
      return VideoTrack::SignKeyword();
    case WebMediaPlayerClient::kVideoTrackKindSubtitles:
      return VideoTrack::SubtitlesKeyword();
    case WebMediaPlayerClient::kVideoTrackKindCommentary:
      return VideoTrack::CommentaryKeyword();
  }

  NOTREACHED();
  return g_empty_atom;
}
