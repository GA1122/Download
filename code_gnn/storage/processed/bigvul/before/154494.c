error::Error GLES2DecoderPassthroughImpl::ProcessReadPixels(bool did_finish) {
  while (!pending_read_pixels_.empty()) {
    const PendingReadPixels& pending_read_pixels = pending_read_pixels_.front();
    if (did_finish || pending_read_pixels.fence->HasCompleted()) {
      using Result = cmds::ReadPixels::Result;
      Result* result = nullptr;
      if (pending_read_pixels.result_shm_id != 0) {
        result = GetSharedMemoryAs<Result*>(
            pending_read_pixels.result_shm_id,
            pending_read_pixels.result_shm_offset, sizeof(*result));
        if (!result) {
          api()->glDeleteBuffersARBFn(1,
                                      &pending_read_pixels.buffer_service_id);
          pending_read_pixels_.pop_front();
          break;
        }
      }

      void* pixels =
          GetSharedMemoryAs<void*>(pending_read_pixels.pixels_shm_id,
                                   pending_read_pixels.pixels_shm_offset,
                                   pending_read_pixels.pixels_size);
      if (!pixels) {
        api()->glDeleteBuffersARBFn(1, &pending_read_pixels.buffer_service_id);
        pending_read_pixels_.pop_front();
        break;
      }

      api()->glBindBufferFn(GL_PIXEL_PACK_BUFFER_ARB,
                            pending_read_pixels.buffer_service_id);
      void* data = nullptr;
      if (feature_info_->feature_flags().map_buffer_range) {
        data = api()->glMapBufferRangeFn(GL_PIXEL_PACK_BUFFER_ARB, 0,
                                         pending_read_pixels.pixels_size,
                                         GL_MAP_READ_BIT);
      } else {
        data = api()->glMapBufferFn(GL_PIXEL_PACK_BUFFER_ARB, GL_READ_ONLY);
      }
      if (!data) {
        InsertError(GL_OUT_OF_MEMORY, "Failed to map pixel pack buffer.");
        pending_read_pixels_.pop_front();
        break;
      }

      memcpy(pixels, data, pending_read_pixels.pixels_size);
      api()->glUnmapBufferFn(GL_PIXEL_PACK_BUFFER_ARB);
      api()->glBindBufferFn(GL_PIXEL_PACK_BUFFER_ARB,
                            resources_->buffer_id_map.GetServiceIDOrInvalid(
                                bound_buffers_[GL_PIXEL_PACK_BUFFER_ARB]));
      api()->glDeleteBuffersARBFn(1, &pending_read_pixels.buffer_service_id);

      if (result != nullptr) {
        result->success = 1;
      }

      pending_read_pixels_.pop_front();
    }
  }

  DCHECK(!did_finish || pending_read_pixels_.empty());
  return error::kNoError;
}
