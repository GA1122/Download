void WebGLRenderingContextBase::TexImageHelperHTMLCanvasElement(
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
    HTMLCanvasElement* canvas,
    const IntRect& source_sub_rectangle,
    GLsizei depth,
    GLint unpack_image_height,
    ExceptionState& exception_state) {
  const char* func_name = GetTexImageFunctionName(function_id);
  if (isContextLost())
    return;

  if (!ValidateHTMLCanvasElement(security_origin, func_name, canvas,
                                 exception_state))
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
                       target, level, internalformat, canvas->width(),
                       canvas->height(), depth, 0, format, type, xoffset,
                       yoffset, zoffset))
    return;

  bool selecting_sub_rectangle = false;
  if (!ValidateTexImageSubRectangle(
          func_name, function_id, canvas, source_sub_rectangle, depth,
          unpack_image_height, &selecting_sub_rectangle)) {
    return;
  }

  if (function_id == kTexImage2D || function_id == kTexSubImage2D) {
    if (!canvas->IsAccelerated() || !CanUseTexImageByGPU(format, type)) {
      TexImageImpl(function_id, target, level, internalformat, xoffset, yoffset,
                   zoffset, format, type,
                   canvas->CopiedImage(kBackBuffer, kPreferAcceleration).get(),
                   WebGLImageConversion::kHtmlDomCanvas, unpack_flip_y_,
                   unpack_premultiply_alpha_, source_sub_rectangle, 1, 0);
      return;
    }

    IntRect adjusted_source_sub_rectangle = source_sub_rectangle;
    if (!unpack_flip_y_) {
      adjusted_source_sub_rectangle.SetY(canvas->height() -
                                         adjusted_source_sub_rectangle.MaxY());
    }

    if (function_id == kTexImage2D) {
      TexImage2DBase(target, level, internalformat,
                     source_sub_rectangle.Width(),
                     source_sub_rectangle.Height(), 0, format, type, nullptr);
      TexImageByGPU(function_id, texture, target, level, 0, 0, 0, canvas,
                    adjusted_source_sub_rectangle);
    } else {
      TexImageByGPU(function_id, texture, target, level, xoffset, yoffset, 0,
                    canvas, adjusted_source_sub_rectangle);
    }
  } else {

    TexImageImpl(function_id, target, level, internalformat, xoffset, yoffset,
                 zoffset, format, type,
                 canvas->CopiedImage(kBackBuffer, kPreferAcceleration).get(),
                 WebGLImageConversion::kHtmlDomCanvas, unpack_flip_y_,
                 unpack_premultiply_alpha_, source_sub_rectangle, depth,
                 unpack_image_height);
  }
}
