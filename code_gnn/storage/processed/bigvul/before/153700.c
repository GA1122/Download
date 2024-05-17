void GLES2Implementation::GetProgramInfoCHROMIUMHelper(
    GLuint program,
    std::vector<int8_t>* result) {
  DCHECK(result);
  helper_->SetBucketSize(kResultBucketId, 0);
  helper_->GetProgramInfoCHROMIUM(program, kResultBucketId);
  GetBucketContents(kResultBucketId, result);
}
