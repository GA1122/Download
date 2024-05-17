void HTMLMediaElement::parseAttribute(
    const AttributeModificationParams& params) {
  const QualifiedName& name = params.name;
  if (name == srcAttr) {
    BLINK_MEDIA_LOG << "parseAttribute(" << (void*)this
                    << ", srcAttr, old=" << params.oldValue
                    << ", new=" << params.newValue << ")";
    if (!params.newValue.isNull()) {
      m_ignorePreloadNone = false;
      invokeLoadAlgorithm();
    }
  } else if (name == controlsAttr) {
    UseCounter::count(document(),
                      UseCounter::HTMLMediaElementControlsAttribute);
    updateControlsVisibility();
  } else if (name == preloadAttr) {
    setPlayerPreload();
  } else if (name == disableremoteplaybackAttr) {
    UseCounter::count(document(), UseCounter::DisableRemotePlaybackAttribute);
    if (params.oldValue != params.newValue) {
      if (m_webMediaPlayer) {
        m_webMediaPlayer->requestRemotePlaybackDisabled(
            !params.newValue.isNull());
      }
      if (mediaControls())
        mediaControls()->onDisableRemotePlaybackAttributeChanged();
    }
  } else {
    HTMLElement::parseAttribute(params);
  }
}
