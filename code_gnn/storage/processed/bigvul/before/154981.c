void WebGLRenderingContextBase::TexImageHelperCanvasRenderingContextHost(
    const SecurityOrigin* security_origin,
    TexImageFunctionID function_id,
    GLenum target,
    GLint level,
    GLint internalformat,
    GLenum format,
    GLenum type,
    GLint xoffset,
    GLint yoffset,
    GLint zoffset,
    CanvasRenderingContextHost* context_host,
    const IntRect& source_sub_rectangle,
    GLsizei depth,
    GLint unpack_image_height,
    ExceptionState& exception_state) {
  const char* func_name = GetTexImageFunctionName(function_id);
  if (isContextLost())
    return;

  if (!ValidateCanvasRenderingContextHost(security_origin, func_name,
                                          context_host, exception_state))
    return;
  WebGLTexture* texture =
      ValidateTexImageBinding(func_name, function_id, target);
  if (!texture)
    return;
  TexImageFunctionType function_type;
  if (function_id == kTexImage2D)
    function_type = kTexImage;
  else
    function_type = kTexSubImage;
  if (!ValidateTexFunc(func_name, function_type, kSourceHTMLCanvasElement,
                       target, level, internalformat,
                       source_sub_rectangle.Width(),
                       source_sub_rectangle.Height(), depth, 0, format, type,
                       xoffset, yoffset, zoffset))
    return;

  bool selecting_sub_rectangle = false;
  if (!ValidateTexImageSubRectangle(
          func_name, function_id, context_host, source_sub_rectangle, depth,
          unpack_image_height, &selecting_sub_rectangle)) {
    return;
  }

  bool is_webgl_canvas = context_host->Is3d();
  WebGLRenderingContextBase* source_canvas_webgl_context = nullptr;
  SourceImageStatus source_image_status = kInvalidSourceImageStatus;
  scoped_refptr<Image> image;

  bool upload_via_gpu =
      (function_id == kTexImage2D || function_id == kTexSubImage2D) &&
      CanUseTexImageViaGPU(format, type);

  if (is_webgl_canvas && upload_via_gpu) {
    source_canvas_webgl_context =
        ToWebGLRenderingContextBase(context_host->RenderingContext());
  } else {
    image = context_host->GetSourceImageForCanvas(
        &source_image_status, kPreferAcceleration,
        FloatSize(source_sub_rectangle.Width(), source_sub_rectangle.Height()));
    if (source_image_status != kNormalSourceImageStatus)
      return;
  }

  upload_via_gpu &= source_canvas_webgl_context ||
                    (image->IsStaticBitmapImage() && image->IsTextureBacked());

  if (upload_via_gpu) {
    AcceleratedStaticBitmapImage* accel_image = nullptr;
    if (image) {
      accel_image = static_cast<AcceleratedStaticBitmapImage*>(
          ToStaticBitmapImage(image.get()));
    }

    IntRect adjusted_source_sub_rectangle = source_sub_rectangle;
    if (!unpack_flip_y_) {
      adjusted_source_sub_rectangle.SetY(context_host->Size().Height() -
                                         adjusted_source_sub_rectangle.MaxY());
    }

    if (function_id == kTexImage2D) {
      TexImage2DBase(target, level, internalformat,
                     source_sub_rectangle.Width(),
                     source_sub_rectangle.Height(), 0, format, type, nullptr);
      TexImageViaGPU(function_id, texture, target, level, 0, 0, 0, accel_image,
                     source_canvas_webgl_context, adjusted_source_sub_rectangle,
                     unpack_premultiply_alpha_, unpack_flip_y_);
    } else {
      TexImageViaGPU(function_id, texture, target, level, xoffset, yoffset, 0,
                     accel_image, source_canvas_webgl_context,
                     adjusted_source_sub_rectangle, unpack_premultiply_alpha_,
                     unpack_flip_y_);
    }
  } else {
    DCHECK(!(function_id == kTexSubImage2D || function_id == kTexSubImage2D) ||
           (depth == 1 && unpack_image_height == 0));
    DCHECK(image);
    TexImageImpl(function_id, target, level, internalformat, xoffset, yoffset,
                 zoffset, format, type, image.get(),
                 WebGLImageConversion::kHtmlDomCanvas, unpack_flip_y_,
                 unpack_premultiply_alpha_, source_sub_rectangle, depth,
                 unpack_image_height);
  }
}
