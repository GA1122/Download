void GLES2Implementation::TexSubImage2DImpl(GLenum target,
                                            GLint level,
                                            GLint xoffset,
                                            GLint yoffset,
                                            GLsizei width,
                                            GLsizei height,
                                            GLenum format,
                                            GLenum type,
                                            uint32_t unpadded_row_size,
                                            const void* pixels,
                                            uint32_t pixels_padded_row_size,
                                            GLboolean internal,
                                            ScopedTransferBufferPtr* buffer,
                                            uint32_t buffer_padded_row_size) {
  DCHECK(buffer);
  DCHECK_GE(level, 0);
  DCHECK_GT(height, 0);
  DCHECK_GT(width, 0);
  DCHECK_GE(xoffset, 0);
  DCHECK_GE(yoffset, 0);

  const int8_t* source = reinterpret_cast<const int8_t*>(pixels);
  while (height) {
    unsigned int desired_size =
        buffer_padded_row_size * (height - 1) + unpadded_row_size;
    if (!buffer->valid() || buffer->size() == 0) {
      buffer->Reset(desired_size);
      if (!buffer->valid()) {
        return;
      }
    }

    GLint num_rows = ComputeNumRowsThatFitInBuffer(
        buffer_padded_row_size, unpadded_row_size, buffer->size(), height);
    num_rows = std::min(num_rows, height);
    CopyRectToBuffer(source, num_rows, unpadded_row_size,
                     pixels_padded_row_size, buffer->address(),
                     buffer_padded_row_size);
    helper_->TexSubImage2D(target, level, xoffset, yoffset, width, num_rows,
                           format, type, buffer->shm_id(), buffer->offset(),
                           internal);
    buffer->Release();
    yoffset += num_rows;
    source += num_rows * pixels_padded_row_size;
    height -= num_rows;
  }
}
