WebMediaPlayer::Preload HTMLMediaElement::PreloadType() const {
  const AtomicString& preload = FastGetAttribute(kPreloadAttr);
  if (DeprecatedEqualIgnoringCase(preload, "none")) {
    UseCounter::Count(GetDocument(), WebFeature::kHTMLMediaElementPreloadNone);
    return WebMediaPlayer::kPreloadNone;
  }

  if (GetDocument().GetSettings() &&
      GetDocument().GetSettings()->GetForcePreloadNoneForMediaElements() &&
      (current_src_.Protocol() != "blob" && current_src_.Protocol() != "data" &&
       current_src_.Protocol() != "file")) {
    UseCounter::Count(GetDocument(),
                      WebFeature::kHTMLMediaElementPreloadForcedNone);
    return WebMediaPlayer::kPreloadNone;
  }

  if (DeprecatedEqualIgnoringCase(preload, "metadata")) {
    UseCounter::Count(GetDocument(),
                      WebFeature::kHTMLMediaElementPreloadMetadata);
    return WebMediaPlayer::kPreloadMetaData;
  }

  if (GetNetworkStateNotifier().IsCellularConnectionType()) {
    UseCounter::Count(GetDocument(),
                      WebFeature::kHTMLMediaElementPreloadForcedMetadata);
    return WebMediaPlayer::kPreloadMetaData;
  }

  if (DeprecatedEqualIgnoringCase(preload, "auto") ||
      DeprecatedEqualIgnoringCase(preload, "")) {
    UseCounter::Count(GetDocument(), WebFeature::kHTMLMediaElementPreloadAuto);
    return WebMediaPlayer::kPreloadAuto;
  }


  UseCounter::Count(GetDocument(), WebFeature::kHTMLMediaElementPreloadDefault);
  return WebMediaPlayer::kPreloadMetaData;
}
