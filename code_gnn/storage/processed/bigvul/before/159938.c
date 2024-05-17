  bool ThumbnailEqualsBytes(const gfx::Image& image,
                            base::RefCountedMemory* bytes) {
    scoped_refptr<base::RefCountedBytes> encoded_image;
    TopSitesImpl::EncodeBitmap(image, &encoded_image);
    return ThumbnailsAreEqual(encoded_image.get(), bytes);
  }
