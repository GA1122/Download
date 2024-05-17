void WebGLRenderingContextBase::TexImageHelperHTMLImageElement(
    SecurityOrigin* security_origin,
    TexImageFunctionID function_id,
    GLenum target,
    GLint level,
    GLint internalformat,
    GLenum format,
    GLenum type,
    GLint xoffset,
    GLint yoffset,
    GLint zoffset,
    HTMLImageElement* image,
    const IntRect& source_image_rect,
    GLsizei depth,
    GLint unpack_image_height,
    ExceptionState& exception_state) {
  const char* func_name = GetTexImageFunctionName(function_id);
  if (isContextLost())
    return;

  if (!ValidateHTMLImageElement(security_origin, func_name, image,
                                exception_state))
    return;
  if (!ValidateTexImageBinding(func_name, function_id, target))
    return;

  RefPtr<Image> image_for_render = image->CachedImage()->GetImage();
  if (image_for_render && image_for_render->IsSVGImage()) {
    if (canvas()) {
      UseCounter::Count(canvas()->GetDocument(), WebFeature::kSVGInWebGL);
    }
    image_for_render =
        DrawImageIntoBuffer(std::move(image_for_render), image->width(),
                            image->height(), func_name);
  }

  TexImageFunctionType function_type;
  if (function_id == kTexImage2D || function_id == kTexImage3D)
    function_type = kTexImage;
  else
    function_type = kTexSubImage;
  if (!image_for_render ||
      !ValidateTexFunc(func_name, function_type, kSourceHTMLImageElement,
                       target, level, internalformat, image_for_render->width(),
                       image_for_render->height(), depth, 0, format, type,
                       xoffset, yoffset, zoffset))
    return;

  TexImageImpl(function_id, target, level, internalformat, xoffset, yoffset,
               zoffset, format, type, image_for_render.Get(),
               WebGLImageConversion::kHtmlDomImage, unpack_flip_y_,
               unpack_premultiply_alpha_, source_image_rect, depth,
               unpack_image_height);
}
