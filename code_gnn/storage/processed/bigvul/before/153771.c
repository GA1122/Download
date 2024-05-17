void GLES2Implementation::MultiDrawArraysInstancedWEBGLHelper(
    GLenum mode,
    const GLint* firsts,
    const GLsizei* counts,
    const GLsizei* instance_counts,
    GLsizei drawcount) {
  DCHECK_GT(drawcount, 0);

  uint32_t buffer_size =
      ComputeCombinedCopySize(drawcount, firsts, counts, instance_counts);
  ScopedTransferBufferPtr buffer(buffer_size, helper_, transfer_buffer_);

  helper_->MultiDrawBeginCHROMIUM(drawcount);
  auto DoMultiDraw = [&](const std::array<uint32_t, 3>& offsets, uint32_t,
                         uint32_t copy_count) {
    helper_->MultiDrawArraysInstancedCHROMIUM(
        mode, buffer.shm_id(), buffer.offset() + offsets[0], buffer.shm_id(),
        buffer.offset() + offsets[1], buffer.shm_id(),
        buffer.offset() + offsets[2], copy_count);
  };
  if (!TransferArraysAndExecute(drawcount, &buffer, DoMultiDraw, firsts, counts,
                                instance_counts)) {
    SetGLError(GL_OUT_OF_MEMORY, "glMultiDrawArraysInstancedWEBGL",
               "out of memory");
  }
  helper_->MultiDrawEndCHROMIUM();
}
