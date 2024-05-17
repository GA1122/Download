  void ImageCapture::ResolveWithMediaTrackConstraints(
    MediaTrackConstraints constraints,
//     ScriptValue constraints,
      ScriptPromiseResolver* resolver) {
    DCHECK(resolver);
    resolver->Resolve(constraints);
 }