ImageCapture::ImageCapture(ExecutionContext* context, MediaStreamTrack* track)
    : ContextLifecycleObserver(context), stream_track_(track) {
  DCHECK(stream_track_);
  DCHECK(!service_.is_bound());

  if (!GetFrame())
    return;

  GetFrame()->GetInterfaceProvider().GetInterface(mojo::MakeRequest(&service_));

  service_.set_connection_error_handler(ConvertToBaseCallback(WTF::Bind(
      &ImageCapture::OnServiceConnectionError, WrapWeakPersistent(this))));

  service_->GetPhotoState(
      stream_track_->Component()->Source()->Id(),
      ConvertToBaseCallback(WTF::Bind(
          &ImageCapture::UpdateMediaTrackCapabilities, WrapPersistent(this))));
}
