GLenum WebGLImageConversion::ComputeImageSizeInBytes(
    GLenum format,
    GLenum type,
    GLsizei width,
    GLsizei height,
    GLsizei depth,
    const PixelStoreParams& params,
    unsigned* image_size_in_bytes,
    unsigned* padding_in_bytes,
    unsigned* skip_size_in_bytes) {
  DCHECK(image_size_in_bytes);
  DCHECK(params.alignment == 1 || params.alignment == 2 ||
         params.alignment == 4 || params.alignment == 8);
  DCHECK_GE(params.row_length, 0);
  DCHECK_GE(params.image_height, 0);
  DCHECK_GE(params.skip_pixels, 0);
  DCHECK_GE(params.skip_rows, 0);
  DCHECK_GE(params.skip_images, 0);
  if (width < 0 || height < 0 || depth < 0)
    return GL_INVALID_VALUE;
  if (!width || !height || !depth) {
    *image_size_in_bytes = 0;
    if (padding_in_bytes)
      *padding_in_bytes = 0;
    if (skip_size_in_bytes)
      *skip_size_in_bytes = 0;
    return GL_NO_ERROR;
  }

  int row_length = params.row_length > 0 ? params.row_length : width;
  int image_height = params.image_height > 0 ? params.image_height : height;

  unsigned bytes_per_component, components_per_pixel;
  if (!ComputeFormatAndTypeParameters(format, type, &bytes_per_component,
                                      &components_per_pixel))
    return GL_INVALID_ENUM;
  unsigned bytes_per_group = bytes_per_component * components_per_pixel;
  CheckedNumeric<uint32_t> checked_value = static_cast<uint32_t>(row_length);
  checked_value *= bytes_per_group;
  if (!checked_value.IsValid())
    return GL_INVALID_VALUE;

  unsigned last_row_size;
  if (params.row_length > 0 && params.row_length != width) {
    CheckedNumeric<uint32_t> tmp = width;
    tmp *= bytes_per_group;
    if (!tmp.IsValid())
      return GL_INVALID_VALUE;
    last_row_size = tmp.ValueOrDie();
  } else {
    last_row_size = checked_value.ValueOrDie();
  }

  unsigned padding = 0;
  CheckedNumeric<uint32_t> checked_residual = checked_value % params.alignment;
  if (!checked_residual.IsValid()) {
    return GL_INVALID_VALUE;
  }
  unsigned residual = checked_residual.ValueOrDie();
  if (residual) {
    padding = params.alignment - residual;
    checked_value += padding;
  }
  if (!checked_value.IsValid())
    return GL_INVALID_VALUE;
  unsigned padded_row_size = checked_value.ValueOrDie();

  CheckedNumeric<uint32_t> rows = image_height;
  rows *= (depth - 1);
  rows += height;
  if (!rows.IsValid())
    return GL_INVALID_VALUE;
  checked_value *= (rows - 1);
  checked_value += last_row_size;
  if (!checked_value.IsValid())
    return GL_INVALID_VALUE;
  *image_size_in_bytes = checked_value.ValueOrDie();
  if (padding_in_bytes)
    *padding_in_bytes = padding;

  CheckedNumeric<uint32_t> skip_size = 0;
  if (params.skip_images > 0) {
    CheckedNumeric<uint32_t> tmp = padded_row_size;
    tmp *= image_height;
    tmp *= params.skip_images;
    if (!tmp.IsValid())
      return GL_INVALID_VALUE;
    skip_size += tmp.ValueOrDie();
  }
  if (params.skip_rows > 0) {
    CheckedNumeric<uint32_t> tmp = padded_row_size;
    tmp *= params.skip_rows;
    if (!tmp.IsValid())
      return GL_INVALID_VALUE;
    skip_size += tmp.ValueOrDie();
  }
  if (params.skip_pixels > 0) {
    CheckedNumeric<uint32_t> tmp = bytes_per_group;
    tmp *= params.skip_pixels;
    if (!tmp.IsValid())
      return GL_INVALID_VALUE;
    skip_size += tmp.ValueOrDie();
  }
  if (!skip_size.IsValid())
    return GL_INVALID_VALUE;
  if (skip_size_in_bytes)
    *skip_size_in_bytes = skip_size.ValueOrDie();

  checked_value += skip_size.ValueOrDie();
  if (!checked_value.IsValid())
    return GL_INVALID_VALUE;
  return GL_NO_ERROR;
}
