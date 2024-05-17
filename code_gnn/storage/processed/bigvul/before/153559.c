void GLES2Implementation::BindBufferBaseHelper(GLenum target,
                                               GLuint index,
                                               GLuint buffer_id) {
  if (UpdateIndexedBufferState(target, index, buffer_id, "glBindBufferBase")) {
    GetIdHandler(SharedIdNamespaces::kBuffers)
        ->MarkAsUsedForBind(this, target, index, buffer_id,
                            &GLES2Implementation::BindBufferBaseStub);
  }
}
