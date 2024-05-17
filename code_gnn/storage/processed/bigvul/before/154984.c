void WebGLRenderingContextBase::TexImageHelperHTMLVideoElement(
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
    HTMLVideoElement* video,
    const IntRect& source_image_rect,
    GLsizei depth,
    GLint unpack_image_height,
    ExceptionState& exception_state) {
  const char* func_name = GetTexImageFunctionName(function_id);
  if (isContextLost())
    return;

  if (!ValidateHTMLVideoElement(security_origin, func_name, video,
                                exception_state))
    return;
  WebGLTexture* texture =
      ValidateTexImageBinding(func_name, function_id, target);
  if (!texture)
    return;
  TexImageFunctionType function_type;
  if (function_id == kTexImage2D || function_id == kTexImage3D)
    function_type = kTexImage;
  else
    function_type = kTexSubImage;
  if (!ValidateTexFunc(func_name, function_type, kSourceHTMLVideoElement,
                       target, level, internalformat, video->videoWidth(),
                       video->videoHeight(), 1, 0, format, type, xoffset,
                       yoffset, zoffset))
    return;

  GLint adjusted_internalformat =
      ConvertTexInternalFormat(internalformat, type);

  WebMediaPlayer::VideoFrameUploadMetadata frame_metadata = {};
  int already_uploaded_id = -1;
  WebMediaPlayer::VideoFrameUploadMetadata* frame_metadata_ptr = nullptr;
  if (RuntimeEnabledFeatures::ExtraWebGLVideoTextureMetadataEnabled()) {
    already_uploaded_id = texture->GetLastUploadedVideoFrameId();
    frame_metadata_ptr = &frame_metadata;
  }

  if (!source_image_rect.IsValid()) {
    SynthesizeGLError(GL_INVALID_OPERATION, func_name,
                      "source sub-rectangle specified via pixel unpack "
                      "parameters is invalid");
    return;
  }
  bool source_image_rect_is_default =
      source_image_rect == SentinelEmptyRect() ||
      source_image_rect ==
          IntRect(0, 0, video->videoWidth(), video->videoHeight());
  const bool use_copyTextureCHROMIUM = function_id == kTexImage2D &&
                                       source_image_rect_is_default &&
                                       depth == 1 && GL_TEXTURE_2D == target &&
                                       CanUseTexImageViaGPU(format, type);
  if (use_copyTextureCHROMIUM) {
    DCHECK(Extensions3DUtil::CanUseCopyTextureCHROMIUM(target));
    DCHECK_EQ(xoffset, 0);
    DCHECK_EQ(yoffset, 0);
    DCHECK_EQ(zoffset, 0);

    if (video->CopyVideoTextureToPlatformTexture(
            ContextGL(), target, texture->Object(), adjusted_internalformat,
            format, type, level, unpack_premultiply_alpha_, unpack_flip_y_,
            already_uploaded_id, frame_metadata_ptr)) {
      texture->UpdateLastUploadedFrame(frame_metadata);
      return;
    }

    if (video->CopyVideoYUVDataToPlatformTexture(
            ContextGL(), target, texture->Object(), adjusted_internalformat,
            format, type, level, unpack_premultiply_alpha_, unpack_flip_y_,
            already_uploaded_id, frame_metadata_ptr)) {
      texture->UpdateLastUploadedFrame(frame_metadata);
      return;
    }
  }

  if (source_image_rect_is_default) {
    ScopedUnpackParametersResetRestore(
        this, unpack_flip_y_ || unpack_premultiply_alpha_);
    if (video->TexImageImpl(
            static_cast<WebMediaPlayer::TexImageFunctionID>(function_id),
            target, ContextGL(), texture->Object(), level,
            adjusted_internalformat, format, type, xoffset, yoffset, zoffset,
            unpack_flip_y_,
            unpack_premultiply_alpha_ &&
                unpack_colorspace_conversion_ == GL_NONE)) {
      texture->ClearLastUploadedFrame();
      return;
    }
  }

  scoped_refptr<Image> image =
      VideoFrameToImage(video, already_uploaded_id, frame_metadata_ptr);
  if (!image)
    return;
  TexImageImpl(function_id, target, level, adjusted_internalformat, xoffset,
               yoffset, zoffset, format, type, image.get(),
               WebGLImageConversion::kHtmlDomVideo, unpack_flip_y_,
               unpack_premultiply_alpha_, source_image_rect, depth,
               unpack_image_height);
  texture->UpdateLastUploadedFrame(frame_metadata);
}
