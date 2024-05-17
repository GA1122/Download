void GLES2Implementation::BindBufferRangeHelper(GLenum target,
                                                GLuint index,
                                                GLuint buffer_id,
                                                GLintptr offset,
                                                GLsizeiptr size) {
  if (UpdateIndexedBufferState(target, index, buffer_id, "glBindBufferRange")) {
    GetIdHandler(SharedIdNamespaces::kBuffers)
        ->MarkAsUsedForBind(this, target, index, buffer_id, offset, size,
                            &GLES2Implementation::BindBufferRangeStub);
  }
}
