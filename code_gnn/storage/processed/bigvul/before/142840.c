void HTMLMediaElement::MediaSourceOpened(WebMediaSource* web_media_source) {
  SetShouldDelayLoadEvent(false);
  media_source_->SetWebMediaSourceAndOpen(base::WrapUnique(web_media_source));
}
