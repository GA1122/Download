void GLES2Implementation::PushGroupMarkerEXT(GLsizei length,
                                             const GLchar* marker) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glPushGroupMarkerEXT(" << length
                     << ", " << marker << ")");
  if (!marker) {
    marker = "";
  }
  SetBucketAsString(kResultBucketId, (length ? std::string(marker, length)
                                             : std::string(marker)));
  helper_->PushGroupMarkerEXT(kResultBucketId);
  helper_->SetBucketSize(kResultBucketId, 0);
  debug_marker_manager_.PushGroup(length ? std::string(marker, length)
                                         : std::string(marker));
}
