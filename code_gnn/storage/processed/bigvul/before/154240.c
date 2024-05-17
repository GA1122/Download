error::Error GLES2DecoderImpl::HandleGetShaderSource(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  const volatile gles2::cmds::GetShaderSource& c =
      *static_cast<const volatile gles2::cmds::GetShaderSource*>(cmd_data);
  GLuint shader_id = c.shader;
  uint32_t bucket_id = static_cast<uint32_t>(c.bucket_id);
  Bucket* bucket = CreateBucket(bucket_id);
  Shader* shader = GetShaderInfoNotProgram(shader_id, "glGetShaderSource");
  if (!shader || shader->source().empty()) {
    bucket->SetSize(0);
    return error::kNoError;
  }
  bucket->SetFromString(shader->source().c_str());
  return error::kNoError;
}
