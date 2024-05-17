String preloadTypeToString(WebMediaPlayer::Preload preloadType) {
  switch (preloadType) {
    case WebMediaPlayer::PreloadNone:
      return "none";
    case WebMediaPlayer::PreloadMetaData:
      return "metadata";
    case WebMediaPlayer::PreloadAuto:
      return "auto";
  }

  NOTREACHED();
  return String();
}
