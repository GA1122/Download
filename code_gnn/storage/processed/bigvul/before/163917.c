ScriptPromise ImageCapture::grabFrame(ScriptState* script_state) {
  ScriptPromiseResolver* resolver = ScriptPromiseResolver::Create(script_state);
  ScriptPromise promise = resolver->Promise();

  if (TrackIsInactive(*stream_track_)) {
    resolver->Reject(DOMException::Create(
        kInvalidStateError, "The associated Track is in an invalid state."));
    return promise;
  }

  if (!frame_grabber_) {
    frame_grabber_ = Platform::Current()->CreateImageCaptureFrameGrabber();
  }

  if (!frame_grabber_) {
    resolver->Reject(DOMException::Create(
        kUnknownError, "Couldn't create platform resources"));
    return promise;
  }

  WebMediaStreamTrack track(stream_track_->Component());
  frame_grabber_->GrabFrame(
      &track, new CallbackPromiseAdapter<ImageBitmap, void>(resolver));

  return promise;
}
