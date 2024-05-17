void ImageCapture::ResolveWithPhotoSettings(ScriptPromiseResolver* resolver) {
  DCHECK(resolver);
  resolver->Resolve(photo_settings_);
}
