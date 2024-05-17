void WebGLRenderingContextBase::TexImageHelperImageBitmap(
    TexImageFunctionID function_id,
    GLenum target,
    GLint level,
    GLint internalformat,
    GLenum format,
    GLenum type,
    GLint xoffset,
    GLint yoffset,
    GLint zoffset,
    ImageBitmap* bitmap,
    const IntRect& source_sub_rect,
    GLsizei depth,
    GLint unpack_image_height,
    ExceptionState& exception_state) {
  const char* func_name = GetTexImageFunctionName(function_id);
  if (isContextLost())
    return;
  if (!ValidateImageBitmap(func_name, bitmap, exception_state))
    return;
  WebGLTexture* texture =
      ValidateTexImageBinding(func_name, function_id, target);
  if (!texture)
    return;

  bool selecting_sub_rectangle = false;
  if (!ValidateTexImageSubRectangle(func_name, function_id, bitmap,
                                    source_sub_rect, depth, unpack_image_height,
                                    &selecting_sub_rectangle)) {
    return;
  }

  TexImageFunctionType function_type;
  if (function_id == kTexImage2D)
    function_type = kTexImage;
  else
    function_type = kTexSubImage;

  GLsizei width = source_sub_rect.Width();
  GLsizei height = source_sub_rect.Height();
  if (!ValidateTexFunc(func_name, function_type, kSourceImageBitmap, target,
                       level, internalformat, width, height, depth, 0, format,
                       type, xoffset, yoffset, zoffset))
    return;
  DCHECK(bitmap->BitmapImage());

  if (function_id != kTexSubImage3D && function_id != kTexImage3D &&
      bitmap->IsAccelerated() && CanUseTexImageByGPU(format, type) &&
      !selecting_sub_rectangle) {
    if (function_id == kTexImage2D) {
      TexImage2DBase(target, level, internalformat, width, height, 0, format,
                     type, nullptr);
      TexImageByGPU(function_id, texture, target, level, 0, 0, 0, bitmap,
                    source_sub_rect);
    } else if (function_id == kTexSubImage2D) {
      TexImageByGPU(function_id, texture, target, level, xoffset, yoffset, 0,
                    bitmap, source_sub_rect);
    }
    return;
  }
  sk_sp<SkImage> sk_image =
      bitmap->BitmapImage()->PaintImageForCurrentFrame().GetSkImage();
  SkPixmap pixmap;
  uint8_t* pixel_data_ptr = nullptr;
  scoped_refptr<Uint8Array> pixel_data;
  bool peek_succeed = sk_image->peekPixels(&pixmap);
  if (peek_succeed) {
    pixel_data_ptr = static_cast<uint8_t*>(pixmap.writable_addr());
  } else {
    pixel_data = bitmap->CopyBitmapData(
        bitmap->IsPremultiplied() ? kPremultiplyAlpha : kUnpremultiplyAlpha);
    pixel_data_ptr = pixel_data->Data();
  }
  Vector<uint8_t> data;
  bool need_conversion = true;
  bool have_peekable_rgba =
      (peek_succeed &&
       pixmap.colorType() == SkColorType::kRGBA_8888_SkColorType);
  bool is_pixel_data_rgba = (have_peekable_rgba || !peek_succeed);
  if (is_pixel_data_rgba && format == GL_RGBA && type == GL_UNSIGNED_BYTE &&
      !selecting_sub_rectangle && depth == 1) {
    need_conversion = false;
  } else {
    if (type == GL_UNSIGNED_INT_10F_11F_11F_REV) {
      type = GL_FLOAT;
    }
    bool is_pixel_data_bgra =
        pixmap.colorType() == SkColorType::kBGRA_8888_SkColorType;
    if ((is_pixel_data_bgra &&
         !WebGLImageConversion::ExtractImageData(
             pixel_data_ptr, WebGLImageConversion::DataFormat::kDataFormatBGRA8,
             bitmap->Size(), source_sub_rect, depth, unpack_image_height,
             format, type, false, false, data)) ||
        (is_pixel_data_rgba &&
         !WebGLImageConversion::ExtractImageData(
             pixel_data_ptr, WebGLImageConversion::DataFormat::kDataFormatRGBA8,
             bitmap->Size(), source_sub_rect, depth, unpack_image_height,
             format, type, false, false, data))) {
      SynthesizeGLError(GL_INVALID_VALUE, func_name, "bad image data");
      return;
    }
  }
  ScopedUnpackParametersResetRestore temporary_reset_unpack(this);
  if (function_id == kTexImage2D) {
    TexImage2DBase(target, level, internalformat, width, height, 0, format,
                   type, need_conversion ? data.data() : pixel_data_ptr);
  } else if (function_id == kTexSubImage2D) {
    ContextGL()->TexSubImage2D(target, level, xoffset, yoffset, width, height,
                               format, type,
                               need_conversion ? data.data() : pixel_data_ptr);
  } else if (function_id == kTexImage3D) {
    ContextGL()->TexImage3D(target, level, internalformat, width, height, depth,
                            0, format, type,
                            need_conversion ? data.data() : pixel_data_ptr);
  } else {
    DCHECK_EQ(function_id, kTexSubImage3D);
    ContextGL()->TexSubImage3D(target, level, xoffset, yoffset, zoffset, width,
                               height, depth, format, type,
                               need_conversion ? data.data() : pixel_data_ptr);
  }
}
