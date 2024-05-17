void ImageCapture::ResolveWithPhotoCapabilities(
    ScriptPromiseResolver* resolver) {
  DCHECK(resolver);
  resolver->Resolve(photo_capabilities_);
 }
