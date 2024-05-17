void ImageCapture::OnMojoGetPhotoState(
    ScriptPromiseResolver* resolver,
    PromiseResolverFunction resolve_function,
    bool trigger_take_photo,
    media::mojom::blink::PhotoStatePtr photo_state) {
  DCHECK(service_requests_.Contains(resolver));

  if (photo_state.is_null()) {
    resolver->Reject(DOMException::Create(kUnknownError, "platform error"));
    service_requests_.erase(resolver);
    return;
  }

  photo_settings_ = PhotoSettings();
  photo_settings_.setImageHeight(photo_state->height->current);
  photo_settings_.setImageWidth(photo_state->width->current);

  photo_capabilities_ = PhotoCapabilities::Create();
  photo_capabilities_->SetRedEyeReduction(photo_state->red_eye_reduction);
  if (photo_state->height->min != 0 || photo_state->height->max != 0) {
    photo_capabilities_->SetImageHeight(
        MediaSettingsRange::Create(std::move(photo_state->height)));
  }
  if (photo_state->width->min != 0 || photo_state->width->max != 0) {
    photo_capabilities_->SetImageWidth(
        MediaSettingsRange::Create(std::move(photo_state->width)));
  }
  if (!photo_state->fill_light_mode.IsEmpty())
    photo_capabilities_->SetFillLightMode(photo_state->fill_light_mode);

  UpdateMediaTrackCapabilities(std::move(photo_state));

  if (trigger_take_photo) {
    service_->TakePhoto(stream_track_->Component()->Source()->Id(),
                        ConvertToBaseCallback(WTF::Bind(
                            &ImageCapture::OnMojoTakePhoto,
                            WrapPersistent(this), WrapPersistent(resolver))));
    return;
  }

  resolve_function(resolver);
  service_requests_.erase(resolver);
}
