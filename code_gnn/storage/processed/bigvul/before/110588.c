error::Error GLES2DecoderImpl::HandleGetProgramInfoCHROMIUM(
    uint32 immediate_data_size, const gles2::GetProgramInfoCHROMIUM& c) {
  GLuint program = static_cast<GLuint>(c.program);
  uint32 bucket_id = c.bucket_id;
  Bucket* bucket = CreateBucket(bucket_id);
  bucket->SetSize(sizeof(ProgramInfoHeader));   
  ProgramManager::ProgramInfo* info = NULL;
  info = GetProgramInfo(program);
  if (!info || !info->IsValid()) {
    return error::kNoError;
  }
  info->GetProgramInfo(program_manager(), bucket);
  return error::kNoError;
}
