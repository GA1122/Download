error::Error GLES2DecoderPassthroughImpl::DoWaitSync(GLuint sync,
                                                     GLbitfield flags,
                                                     GLuint64 timeout) {
  api()->glWaitSyncFn(GetSyncServiceID(sync, resources_), flags, timeout);
  return error::kNoError;
}
