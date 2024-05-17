void GLES2Implementation::MultiDrawArraysWEBGLHelper(GLenum mode,
                                                     const GLint* firsts,
                                                     const GLsizei* counts,
                                                     GLsizei drawcount) {
  DCHECK_GT(drawcount, 0);

  uint32_t buffer_size = ComputeCombinedCopySize(drawcount, firsts, counts);
  ScopedTransferBufferPtr buffer(buffer_size, helper_, transfer_buffer_);

  helper_->MultiDrawBeginCHROMIUM(drawcount);
  auto DoMultiDraw = [&](const std::array<uint32_t, 2>& offsets, uint32_t,
                         uint32_t copy_count) {
    helper_->MultiDrawArraysCHROMIUM(
        mode, buffer.shm_id(), buffer.offset() + offsets[0], buffer.shm_id(),
        buffer.offset() + offsets[1], copy_count);
  };
  if (!TransferArraysAndExecute(drawcount, &buffer, DoMultiDraw, firsts,
                                counts)) {
    SetGLError(GL_OUT_OF_MEMORY, "glMultiDrawArraysWEBGL", "out of memory");
  }
  helper_->MultiDrawEndCHROMIUM();
}
