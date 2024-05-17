void GLES2Implementation::DeleteSyncHelper(GLsync sync) {
  GLuint sync_uint = ToGLuint(sync);
  if (!GetIdHandler(SharedIdNamespaces::kSyncs)
           ->FreeIds(this, 1, &sync_uint,
                     &GLES2Implementation::DeleteSyncStub)) {
    SetGLError(GL_INVALID_VALUE, "glDeleteSync",
               "id not created by this context.");
  }
}
