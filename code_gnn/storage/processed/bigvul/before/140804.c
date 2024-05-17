error::Error GLES2DecoderImpl::HandleGetShaderSource(uint32 immediate_data_size,
                                                     const void* cmd_data) {
  const gles2::cmds::GetShaderSource& c =
      *static_cast<const gles2::cmds::GetShaderSource*>(cmd_data);
  GLuint shader_id = c.shader;
  uint32 bucket_id = static_cast<uint32>(c.bucket_id);
  Bucket* bucket = CreateBucket(bucket_id);
  Shader* shader = GetShaderInfoNotProgram(shader_id, "glGetShaderSource");
  if (!shader || shader->source().empty()) {
    bucket->SetSize(0);
    return error::kNoError;
  }
  bucket->SetFromString(shader->source().c_str());
  return error::kNoError;
}
