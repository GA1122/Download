String PreloadTypeToString(WebMediaPlayer::Preload preload_type) {
  switch (preload_type) {
    case WebMediaPlayer::kPreloadNone:
      return "none";
    case WebMediaPlayer::kPreloadMetaData:
      return "metadata";
    case WebMediaPlayer::kPreloadAuto:
      return "auto";
  }

  NOTREACHED();
  return String();
}
