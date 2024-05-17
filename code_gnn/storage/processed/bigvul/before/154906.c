  ScopedUnpackStateButAlignmentReset(gl::GLApi* api, bool enable, bool is_3d)
      : api_(api) {
    if (!enable) {
      return;
    }

    api_->glGetIntegervFn(GL_UNPACK_SKIP_PIXELS, &skip_pixels_);
    api_->glPixelStoreiFn(GL_UNPACK_SKIP_PIXELS, 0);
    api_->glGetIntegervFn(GL_UNPACK_SKIP_ROWS, &skip_rows_);
    api_->glPixelStoreiFn(GL_UNPACK_SKIP_ROWS, 0);
    api_->glGetIntegervFn(GL_UNPACK_ROW_LENGTH, &row_length_);
    api_->glPixelStoreiFn(GL_UNPACK_ROW_LENGTH, 0);

    if (is_3d) {
      api_->glGetIntegervFn(GL_UNPACK_SKIP_IMAGES, &skip_images_);
      api_->glPixelStoreiFn(GL_UNPACK_SKIP_IMAGES, 0);
      api_->glGetIntegervFn(GL_UNPACK_IMAGE_HEIGHT, &image_height_);
      api_->glPixelStoreiFn(GL_UNPACK_IMAGE_HEIGHT, 0);
    }
  }
