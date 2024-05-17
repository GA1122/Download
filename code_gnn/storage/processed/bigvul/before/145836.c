std::string EncodeBitmap(const SkBitmap& bitmap,
                         ImageEncoding encoding,
                         int quality) {
  gfx::Image image = gfx::Image::CreateFrom1xBitmap(bitmap);
  DCHECK(!image.IsEmpty());

  scoped_refptr<base::RefCountedMemory> data;
  if (encoding == ImageEncoding::kPng) {
    data = image.As1xPNGBytes();
  } else if (encoding == ImageEncoding::kJpeg) {
    scoped_refptr<base::RefCountedBytes> bytes(new base::RefCountedBytes());
    if (gfx::JPEG1xEncodedDataFromImage(image, quality, &bytes->data()))
      data = bytes;
  }

  if (!data || !data->front())
    return std::string();

  std::string base_64_data;
  base::Base64Encode(
      base::StringPiece(reinterpret_cast<const char*>(data->front()),
                        data->size()),
      &base_64_data);

  return base_64_data;
}
