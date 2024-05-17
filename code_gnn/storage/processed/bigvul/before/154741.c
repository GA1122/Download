error::Error GLES2DecoderPassthroughImpl::DoIsSync(GLuint sync,
                                                   uint32_t* result) {
  *result = api()->glIsSyncFn(GetSyncServiceID(sync, resources_));
  return error::kNoError;
}
