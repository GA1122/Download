error::Error GLES2DecoderImpl::HandleGetProgramInfoCHROMIUM(
    uint32 immediate_data_size,
    const void* cmd_data) {
  const gles2::cmds::GetProgramInfoCHROMIUM& c =
      *static_cast<const gles2::cmds::GetProgramInfoCHROMIUM*>(cmd_data);
  GLuint program_id = static_cast<GLuint>(c.program);
  uint32 bucket_id = c.bucket_id;
  Bucket* bucket = CreateBucket(bucket_id);
  bucket->SetSize(sizeof(ProgramInfoHeader));   
  Program* program = NULL;
  program = GetProgram(program_id);
  if (!program || !program->IsValid()) {
    return error::kNoError;
  }
  program->GetProgramInfo(program_manager(), bucket);
  return error::kNoError;
}
