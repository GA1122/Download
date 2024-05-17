void ImageCapture::ResolveWithNothing(ScriptPromiseResolver* resolver) {
  DCHECK(resolver);
  resolver->Resolve();
}
