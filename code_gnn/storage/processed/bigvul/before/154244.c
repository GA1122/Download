error::Error GLES2DecoderImpl::HandleGetTranslatedShaderSourceANGLE(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  const volatile gles2::cmds::GetTranslatedShaderSourceANGLE& c =
      *static_cast<const volatile gles2::cmds::GetTranslatedShaderSourceANGLE*>(
          cmd_data);
  GLuint shader_id = c.shader;
  uint32_t bucket_id = static_cast<uint32_t>(c.bucket_id);
  Bucket* bucket = CreateBucket(bucket_id);
  Shader* shader = GetShaderInfoNotProgram(
      shader_id, "glGetTranslatedShaderSourceANGLE");
  if (!shader) {
    bucket->SetSize(0);
    return error::kNoError;
  }

  CompileShaderAndExitCommandProcessingEarly(shader);

  bucket->SetFromString(shader->translated_source().c_str());
  return error::kNoError;
}
