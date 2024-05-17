error::Error GLES2DecoderImpl::HandleTraceBeginCHROMIUM(
    uint32 immediate_data_size, const cmds::TraceBeginCHROMIUM& c) {
  Bucket* bucket = GetBucket(c.bucket_id);
  if (!bucket || bucket->size() == 0) {
    return error::kInvalidArguments;
  }
  std::string command_name;
  if (!bucket->GetAsString(&command_name)) {
    return error::kInvalidArguments;
  }
  TRACE_EVENT_COPY_ASYNC_BEGIN0("gpu", command_name.c_str(), this);
  if (!gpu_tracer_->Begin(command_name, kTraceCHROMIUM)) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        "glTraceBeginCHROMIUM", "unable to create begin trace");
    return error::kNoError;
  }
  return error::kNoError;
}
