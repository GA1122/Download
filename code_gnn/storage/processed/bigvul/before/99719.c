bool VaapiWrapper::UploadVideoFrameToSurface(
    const scoped_refptr<media::VideoFrame>& frame,
    VASurfaceID va_surface_id) {
  base::AutoLock auto_lock(*va_lock_);

  VAImage image;
  VAStatus va_res = vaDeriveImage(va_display_, va_surface_id, &image);
  VA_SUCCESS_OR_RETURN(va_res, "vaDeriveImage failed", false);
  base::ScopedClosureRunner vaimage_deleter(
      base::Bind(&DestroyVAImage, va_display_, image));

  if (image.format.fourcc != VA_FOURCC_NV12) {
    LOG(ERROR) << "Unsupported image format: " << image.format.fourcc;
    return false;
  }

  if (gfx::Rect(image.width, image.height) < gfx::Rect(frame->coded_size())) {
    LOG(ERROR) << "Buffer too small to fit the frame.";
    return false;
  }

  void* image_ptr = NULL;
  va_res = vaMapBuffer(va_display_, image.buf, &image_ptr);
  VA_SUCCESS_OR_RETURN(va_res, "vaMapBuffer failed", false);
  DCHECK(image_ptr);

  int ret = 0;
  {
    base::AutoUnlock auto_unlock(*va_lock_);
    ret = libyuv::I420ToNV12(frame->data(media::VideoFrame::kYPlane),
                             frame->stride(media::VideoFrame::kYPlane),
                             frame->data(media::VideoFrame::kUPlane),
                             frame->stride(media::VideoFrame::kUPlane),
                             frame->data(media::VideoFrame::kVPlane),
                             frame->stride(media::VideoFrame::kVPlane),
                             static_cast<uint8*>(image_ptr) + image.offsets[0],
                             image.pitches[0],
                             static_cast<uint8*>(image_ptr) + image.offsets[1],
                             image.pitches[1],
                             image.width,
                             image.height);
  }

  va_res = vaUnmapBuffer(va_display_, image.buf);
  VA_LOG_ON_ERROR(va_res, "vaUnmapBuffer failed");

  return ret == 0;
}
