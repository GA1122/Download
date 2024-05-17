error::Error GLES2DecoderPassthroughImpl::DoClientWaitSync(GLuint sync,
                                                           GLbitfield flags,
                                                           GLuint64 timeout,
                                                           GLenum* result) {
  GLbitfield modified_flags = flags | GL_SYNC_FLUSH_COMMANDS_BIT;
  *result = api()->glClientWaitSyncFn(GetSyncServiceID(sync, resources_),
                                      modified_flags, timeout);
  return error::kNoError;
}
