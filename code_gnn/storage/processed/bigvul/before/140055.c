WebMediaPlayer::Preload HTMLMediaElement::preloadType() const {
  const AtomicString& preload = fastGetAttribute(preloadAttr);
  if (equalIgnoringCase(preload, "none")) {
    UseCounter::count(document(), UseCounter::HTMLMediaElementPreloadNone);
    return WebMediaPlayer::PreloadNone;
  }

  if (document().settings() &&
      (document().settings()->getDataSaverEnabled() ||
       document().settings()->getForcePreloadNoneForMediaElements()) &&
      (m_currentSrc.protocol() != "blob" && m_currentSrc.protocol() != "data" &&
       m_currentSrc.protocol() != "file")) {
    UseCounter::count(document(),
                      UseCounter::HTMLMediaElementPreloadForcedNone);
    return WebMediaPlayer::PreloadNone;
  }

  if (equalIgnoringCase(preload, "metadata")) {
    UseCounter::count(document(), UseCounter::HTMLMediaElementPreloadMetadata);
    return WebMediaPlayer::PreloadMetaData;
  }

  if (networkStateNotifier().isCellularConnectionType()) {
    UseCounter::count(document(),
                      UseCounter::HTMLMediaElementPreloadForcedMetadata);
    return WebMediaPlayer::PreloadMetaData;
  }

  if (equalIgnoringCase(preload, "auto")) {
    UseCounter::count(document(), UseCounter::HTMLMediaElementPreloadAuto);
    return WebMediaPlayer::PreloadAuto;
  }



  UseCounter::count(document(), UseCounter::HTMLMediaElementPreloadDefault);
  return WebMediaPlayer::PreloadAuto;
}
