void GLES2Implementation::TexSubImage3DImpl(GLenum target,
                                            GLint level,
                                            GLint xoffset,
                                            GLint yoffset,
                                            GLsizei zoffset,
                                            GLsizei width,
                                            GLsizei height,
                                            GLsizei depth,
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
  DCHECK_GT(width, 0);
  DCHECK_GT(height, 0);
  DCHECK_GT(depth, 0);
  DCHECK_GE(xoffset, 0);
  DCHECK_GE(yoffset, 0);
  DCHECK_GE(zoffset, 0);
  const int8_t* source = reinterpret_cast<const int8_t*>(pixels);
  GLsizei total_rows = height * depth;
  GLint row_index = 0, depth_index = 0;
  while (total_rows) {
    GLsizei max_rows;
    unsigned int desired_size;
    if (row_index > 0) {
      max_rows = height - row_index;
      if (total_rows <= height) {
        desired_size =
            buffer_padded_row_size * (max_rows - 1) + unpadded_row_size;
      } else {
        desired_size = buffer_padded_row_size * max_rows;
      }
    } else {
      max_rows = total_rows;
      desired_size =
          buffer_padded_row_size * (max_rows - 1) + unpadded_row_size;
    }
    if (!buffer->valid() || buffer->size() == 0) {
      buffer->Reset(desired_size);
      if (!buffer->valid()) {
        return;
      }
    }
    GLint num_rows = ComputeNumRowsThatFitInBuffer(
        buffer_padded_row_size, unpadded_row_size, buffer->size(), total_rows);
    num_rows = std::min(num_rows, max_rows);
    GLint num_images = num_rows / height;
    GLsizei my_height, my_depth;
    if (num_images > 0) {
      num_rows = num_images * height;
      my_height = height;
      my_depth = num_images;
    } else {
      my_height = num_rows;
      my_depth = 1;
    }

    if (num_images > 0) {
      int8_t* buffer_pointer = reinterpret_cast<int8_t*>(buffer->address());
      uint32_t src_height =
          unpack_image_height_ > 0 ? unpack_image_height_ : height;
      uint32_t image_size_dst = buffer_padded_row_size * height;
      uint32_t image_size_src = pixels_padded_row_size * src_height;
      for (GLint ii = 0; ii < num_images; ++ii) {
        CopyRectToBuffer(source + ii * image_size_src, my_height,
                         unpadded_row_size, pixels_padded_row_size,
                         buffer_pointer + ii * image_size_dst,
                         buffer_padded_row_size);
      }
    } else {
      CopyRectToBuffer(source, my_height, unpadded_row_size,
                       pixels_padded_row_size, buffer->address(),
                       buffer_padded_row_size);
    }
    helper_->TexSubImage3D(target, level, xoffset, yoffset + row_index,
                           zoffset + depth_index, width, my_height, my_depth,
                           format, type, buffer->shm_id(), buffer->offset(),
                           internal);
    buffer->Release();

    total_rows -= num_rows;
    if (total_rows > 0) {
      GLint num_image_paddings;
      if (num_images > 0) {
        DCHECK_EQ(row_index, 0);
        depth_index += num_images;
        num_image_paddings = num_images;
      } else {
        row_index = (row_index + my_height) % height;
        num_image_paddings = 0;
        if (my_height > 0 && row_index == 0) {
          depth_index++;
          num_image_paddings++;
        }
      }
      source += num_rows * pixels_padded_row_size;
      if (unpack_image_height_ > height && num_image_paddings > 0) {
        source += num_image_paddings * (unpack_image_height_ - height) *
                  pixels_padded_row_size;
      }
    }
  }
}
