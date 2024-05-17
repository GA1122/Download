void VaapiWrapper::ReturnVaImage(VAImage* image) {
  base::AutoLock auto_lock(*va_lock_);

  VAStatus va_res = vaUnmapBuffer(va_display_, image->buf);
  VA_LOG_ON_ERROR(va_res, "vaUnmapBuffer failed");

  va_res = vaDestroyImage(va_display_, image->image_id);
  VA_LOG_ON_ERROR(va_res, "vaDestroyImage failed");
}
