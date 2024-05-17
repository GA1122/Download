ScriptPromise ImageCapture::setOptions(ScriptState* script_state,
                                       const PhotoSettings& photo_settings,
                                       bool trigger_take_photo  ) {
  ScriptPromiseResolver* resolver = ScriptPromiseResolver::Create(script_state);
  ScriptPromise promise = resolver->Promise();

  if (TrackIsInactive(*stream_track_)) {
    resolver->Reject(DOMException::Create(
        kInvalidStateError, "The associated Track is in an invalid state."));
    return promise;
  }

  if (!service_) {
    resolver->Reject(DOMException::Create(kNotFoundError, kNoServiceError));
    return promise;
  }
  service_requests_.insert(resolver);

  auto settings = media::mojom::blink::PhotoSettings::New();

  settings->has_height = photo_settings.hasImageHeight();
  if (settings->has_height) {
    const double height = photo_settings.imageHeight();
    if (photo_capabilities_ &&
        (height < photo_capabilities_->imageHeight()->min() ||
         height > photo_capabilities_->imageHeight()->max())) {
      resolver->Reject(DOMException::Create(
          kNotSupportedError, "imageHeight setting out of range"));
      return promise;
    }
    settings->height = height;
  }
  settings->has_width = photo_settings.hasImageWidth();
  if (settings->has_width) {
    const double width = photo_settings.imageWidth();
    if (photo_capabilities_ &&
        (width < photo_capabilities_->imageWidth()->min() ||
         width > photo_capabilities_->imageWidth()->max())) {
      resolver->Reject(DOMException::Create(kNotSupportedError,
                                            "imageWidth setting out of range"));
      return promise;
    }
    settings->width = width;
  }

  settings->has_red_eye_reduction = photo_settings.hasRedEyeReduction();
  if (settings->has_red_eye_reduction) {
    if (photo_capabilities_ &&
        !photo_capabilities_->IsRedEyeReductionControllable()) {
      resolver->Reject(DOMException::Create(
          kNotSupportedError, "redEyeReduction is not controllable."));
      return promise;
    }
    settings->red_eye_reduction = photo_settings.redEyeReduction();
  }

  settings->has_fill_light_mode = photo_settings.hasFillLightMode();
  if (settings->has_fill_light_mode) {
    const String fill_light_mode = photo_settings.fillLightMode();
    if (photo_capabilities_ && photo_capabilities_->fillLightMode().Find(
                                   fill_light_mode) == kNotFound) {
      resolver->Reject(DOMException::Create(kNotSupportedError,
                                            "Unsupported fillLightMode"));
      return promise;
    }
    settings->fill_light_mode = ParseFillLightMode(fill_light_mode);
  }

  auto resolver_cb =
      WTF::Bind(&ImageCapture::ResolveWithNothing, WrapPersistent(this));

  service_->SetOptions(
      stream_track_->Component()->Source()->Id(), std::move(settings),
      ConvertToBaseCallback(
          WTF::Bind(&ImageCapture::OnMojoSetOptions, WrapPersistent(this),
                    WrapPersistent(resolver),
                    WTF::Passed(std::move(resolver_cb)), trigger_take_photo)));
  return promise;
}
