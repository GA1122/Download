void ImageBitmapFactories::ImageBitmapLoader::ResolvePromiseOnOriginalThread(
    sk_sp<SkImage> frame) {
  if (!frame) {
    RejectPromise(kUndecodableImageBitmapRejectionReason);
    return;
  }
  DCHECK(frame->width());
  DCHECK(frame->height());

  scoped_refptr<StaticBitmapImage> image =
      StaticBitmapImage::Create(std::move(frame));
  image->SetOriginClean(true);
  ImageBitmap* image_bitmap = ImageBitmap::Create(image, crop_rect_, options_);
  if (image_bitmap && image_bitmap->BitmapImage()) {
    resolver_->Resolve(image_bitmap);
  } else {
    RejectPromise(kAllocationFailureImageBitmapRejectionReason);
    return;
  }
  factory_->DidFinishLoading(this);
 }
