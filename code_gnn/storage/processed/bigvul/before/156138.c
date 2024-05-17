AcceleratedStaticBitmapImage::CreateFromSkImage(
    sk_sp<SkImage> image,
    base::WeakPtr<WebGraphicsContext3DProviderWrapper>
        context_provider_wrapper) {
  CHECK(image && image->isTextureBacked());
  return base::AdoptRef(new AcceleratedStaticBitmapImage(
      std::move(image), std::move(context_provider_wrapper)));
}
