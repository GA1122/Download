void HTMLMediaElement::LoadSourceFromAttribute() {
  load_state_ = kLoadingFromSrcAttr;
  const AtomicString& src_value = FastGetAttribute(kSrcAttr);

  if (src_value.IsEmpty()) {
    BLINK_MEDIA_LOG << "LoadSourceFromAttribute(" << (void*)this
                    << "), empty 'src'";
    MediaLoadingFailed(WebMediaPlayer::kNetworkStateFormatError,
                       BuildElementErrorMessage("Empty src attribute"));
    return;
  }

  KURL media_url = GetDocument().CompleteURL(src_value);
  if (!IsSafeToLoadURL(media_url, kComplain)) {
    MediaLoadingFailed(
        WebMediaPlayer::kNetworkStateFormatError,
        BuildElementErrorMessage("Media load rejected by URL safety check"));
    return;
  }

  LoadResource(WebMediaPlayerSource(WebURL(media_url)), String());
}
