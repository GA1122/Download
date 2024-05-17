error::Error GLES2DecoderImpl::HandleGetShaderInfoLog(
    uint32 immediate_data_size,
    const void* cmd_data) {
  const gles2::cmds::GetShaderInfoLog& c =
      *static_cast<const gles2::cmds::GetShaderInfoLog*>(cmd_data);
  GLuint shader_id = c.shader;
  uint32 bucket_id = static_cast<uint32>(c.bucket_id);
  Bucket* bucket = CreateBucket(bucket_id);
  Shader* shader = GetShaderInfoNotProgram(shader_id, "glGetShaderInfoLog");
  if (!shader) {
    bucket->SetFromString("");
    return error::kNoError;
  }

  shader->DoCompile();

  bucket->SetFromString(shader->log_info().c_str());
  return error::kNoError;
}
