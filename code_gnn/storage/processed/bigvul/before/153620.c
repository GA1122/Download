void GLES2Implementation::DeleteSyncStub(GLsizei n, const GLuint* syncs) {
  DCHECK_EQ(1, n);
  helper_->DeleteSync(syncs[0]);
}
