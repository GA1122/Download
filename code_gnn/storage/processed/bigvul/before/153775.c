void GLES2Implementation::MultiDrawElementsInstancedWEBGLHelper(
    GLenum mode,
    const GLsizei* counts,
    GLenum type,
    const GLsizei* offsets,
    const GLsizei* instance_counts,
    GLsizei drawcount) {
  DCHECK_GT(drawcount, 0);

  uint32_t buffer_size =
      ComputeCombinedCopySize(drawcount, counts, offsets, instance_counts);
  ScopedTransferBufferPtr buffer(buffer_size, helper_, transfer_buffer_);

  helper_->MultiDrawBeginCHROMIUM(drawcount);
  auto DoMultiDraw = [&](const std::array<uint32_t, 3>& offsets, uint32_t,
                         uint32_t copy_count) {
    helper_->MultiDrawElementsInstancedCHROMIUM(
        mode, buffer.shm_id(), buffer.offset() + offsets[0], type,
        buffer.shm_id(), buffer.offset() + offsets[1], buffer.shm_id(),
        buffer.offset() + offsets[2], copy_count);
  };
  if (!TransferArraysAndExecute(drawcount, &buffer, DoMultiDraw, counts,
                                offsets, instance_counts)) {
    SetGLError(GL_OUT_OF_MEMORY, "glMultiDrawElementsInstancedWEBGL",
               "out of memory");
  }
  helper_->MultiDrawEndCHROMIUM();
}
