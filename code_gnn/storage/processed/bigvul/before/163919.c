ScriptPromise ImageCapture::takePhoto(ScriptState* script_state) {
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

  service_->TakePhoto(stream_track_->Component()->Source()->Id(),
                      ConvertToBaseCallback(WTF::Bind(
                          &ImageCapture::OnMojoTakePhoto, WrapPersistent(this),
                          WrapPersistent(resolver))));
  return promise;
}
