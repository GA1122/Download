error::Error GLES2DecoderImpl::HandleGetShaderSource(
    uint32 immediate_data_size, const cmds::GetShaderSource& c) {
  GLuint shader_id = c.shader;
  uint32 bucket_id = static_cast<uint32>(c.bucket_id);
  Bucket* bucket = CreateBucket(bucket_id);
  Shader* shader = GetShaderInfoNotProgram(shader_id, "glGetShaderSource");
  if (!shader || !shader->source()) {
    bucket->SetSize(0);
    return error::kNoError;
  }
  bucket->SetFromString(shader->source()->c_str());
  return error::kNoError;
}
