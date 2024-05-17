void ImageBitmapFactories::DidFinishLoading(ImageBitmapLoader* loader) {
  DCHECK(pending_loaders_.Contains(loader));
   pending_loaders_.erase(loader);
 }
