void GLES2Implementation::TraceBeginCHROMIUM(const char* category_name,
                                             const char* trace_name) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glTraceBeginCHROMIUM("
                     << category_name << ", " << trace_name << ")");
  static constexpr size_t kMaxStrLen = 256;
  DCHECK_LE(strlen(category_name), kMaxStrLen);
  DCHECK_LE(strlen(trace_name), kMaxStrLen);
  SetBucketAsCString(kResultBucketId, category_name);
  SetBucketAsCString(kResultBucketId + 1, trace_name);
  helper_->TraceBeginCHROMIUM(kResultBucketId, kResultBucketId + 1);
  helper_->SetBucketSize(kResultBucketId, 0);
  helper_->SetBucketSize(kResultBucketId + 1, 0);
  current_trace_stack_++;
}
