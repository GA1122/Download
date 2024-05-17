void ImageCapture::OnMojoSetOptions(ScriptPromiseResolver* resolver,
                                    PromiseResolverFunction resolve_function,
                                    bool trigger_take_photo,
                                    bool result) {
  DCHECK(service_requests_.Contains(resolver));

  if (!result) {
    resolver->Reject(DOMException::Create(kUnknownError, "setOptions failed"));
    service_requests_.erase(resolver);
    return;
  }

  service_->GetPhotoState(
      stream_track_->Component()->Source()->Id(),
      ConvertToBaseCallback(WTF::Bind(
          &ImageCapture::OnMojoGetPhotoState, WrapPersistent(this),
          WrapPersistent(resolver), WTF::Passed(std::move(resolve_function)),
          trigger_take_photo)));
}
