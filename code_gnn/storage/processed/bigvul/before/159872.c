bool TopSitesImpl::EncodeBitmap(const gfx::Image& bitmap,
                                scoped_refptr<base::RefCountedBytes>* bytes) {
  if (bitmap.IsEmpty())
    return false;
  *bytes = new base::RefCountedBytes();
  if (!gfx::JPEG1xEncodedDataFromImage(bitmap, kTopSitesImageQuality,
                                       &(*bytes)->data())) {
    return false;
  }

  (*bytes)->data().shrink_to_fit();
  return true;
}
