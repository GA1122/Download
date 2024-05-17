void GLES2Implementation::MultiDrawElementsWEBGLHelper(GLenum mode,
                                                       const GLsizei* counts,
                                                       GLenum type,
                                                       const GLsizei* offsets,
                                                       GLsizei drawcount) {
  DCHECK_GT(drawcount, 0);

  uint32_t buffer_size = ComputeCombinedCopySize(drawcount, counts, offsets);
  ScopedTransferBufferPtr buffer(buffer_size, helper_, transfer_buffer_);

  helper_->MultiDrawBeginCHROMIUM(drawcount);
  auto DoMultiDraw = [&](const std::array<uint32_t, 2>& offsets, uint32_t,
                         uint32_t copy_count) {
    helper_->MultiDrawElementsCHROMIUM(
        mode, buffer.shm_id(), buffer.offset() + offsets[0], type,
        buffer.shm_id(), buffer.offset() + offsets[1], copy_count);
  };
  if (!TransferArraysAndExecute(drawcount, &buffer, DoMultiDraw, counts,
                                offsets)) {
    SetGLError(GL_OUT_OF_MEMORY, "glMultiDrawElementsWEBGL", "out of memory");
  }
  helper_->MultiDrawEndCHROMIUM();
}
