error::Error GLES2DecoderImpl::HandleEndQueryEXT(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  const volatile gles2::cmds::EndQueryEXT& c =
      *static_cast<const volatile gles2::cmds::EndQueryEXT*>(cmd_data);
  GLenum target = static_cast<GLenum>(c.target);
  uint32_t submit_count = static_cast<GLuint>(c.submit_count);

  QueryManager::Query* query = query_manager_->GetActiveQuery(target);
  if (!query) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION, "glEndQueryEXT", "No active query");
    return error::kNoError;
  }

  if (target == GL_READBACK_SHADOW_COPIES_UPDATED_CHROMIUM &&
      !writes_submitted_but_not_completed_.empty()) {
    query->AddCallback(
        base::BindOnce(&GLES2DecoderImpl::ReadBackBuffersIntoShadowCopies,
                       base::Unretained(this),
                       std::move(writes_submitted_but_not_completed_)));
    writes_submitted_but_not_completed_.clear();
  }

  query_manager_->EndQuery(query, submit_count);
  return error::kNoError;
}
