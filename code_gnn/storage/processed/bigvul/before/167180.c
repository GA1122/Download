void HTMLMediaElement::StartPlayerLoad() {
  DCHECK(!web_media_player_);

  WebMediaPlayerSource source;
  if (src_object_) {
    source = WebMediaPlayerSource(WebMediaStream(src_object_));
  } else {
    KURL request_url = current_src_;
    if (!request_url.User().IsEmpty())
      request_url.SetUser(String());
    if (!request_url.Pass().IsEmpty())
      request_url.SetPass(String());

    KURL kurl(request_url);
    source = WebMediaPlayerSource(WebURL(kurl));
  }

  LocalFrame* frame = GetDocument().GetFrame();
  if (!frame) {
    MediaLoadingFailed(
        WebMediaPlayer::kNetworkStateFormatError,
        BuildElementErrorMessage("Player load failure: document has no frame"));
    return;
  }

  web_media_player_ = frame->Client()->CreateWebMediaPlayer(
      *this, source, this,
      frame->GetPage()->GetChromeClient().GetWebLayerTreeView(frame));

  if (!web_media_player_) {
    MediaLoadingFailed(WebMediaPlayer::kNetworkStateFormatError,
                       BuildElementErrorMessage(
                           "Player load failure: error creating media player"));
    return;
  }

  if (GetLayoutObject())
    GetLayoutObject()->SetShouldDoFullPaintInvalidation();
  audio_source_provider_.Wrap(web_media_player_->GetAudioSourceProvider());
  web_media_player_->SetVolume(EffectiveMediaVolume());

  web_media_player_->SetPoster(PosterImageURL());

  web_media_player_->SetPreload(EffectivePreloadType());

  web_media_player_->RequestRemotePlaybackDisabled(
      FastHasAttribute(disableremoteplaybackAttr));

  web_media_player_->Load(GetLoadType(), source, CorsMode());

  if (IsFullscreen())
    web_media_player_->EnteredFullscreen();

  web_media_player_->BecameDominantVisibleContent(mostly_filling_viewport_);
}
