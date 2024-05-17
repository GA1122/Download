GLboolean GLES2Implementation::IsQueryEXT(GLuint id) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] IsQueryEXT(" << id << ")");

  return query_tracker_->GetQuery(id) != nullptr;
}
