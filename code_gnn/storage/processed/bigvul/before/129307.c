void GLES2DecoderImpl::DoRegisterSharedIdsCHROMIUM(
    GLuint namespace_id, GLsizei n, const GLuint* ids) {
  IdAllocatorInterface* id_allocator = group_->GetIdAllocator(namespace_id);
  for (GLsizei ii = 0; ii < n; ++ii) {
    if (!id_allocator->MarkAsUsed(ids[ii])) {
      for (GLsizei jj = 0; jj < ii; ++jj) {
        id_allocator->FreeID(ids[jj]);
      }
      LOCAL_SET_GL_ERROR(
          GL_INVALID_VALUE, "RegisterSharedIdsCHROMIUM",
          "attempt to register id that already exists");
      return;
    }
  }
}
