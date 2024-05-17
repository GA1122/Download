void GLES2Implementation::GetUniformsES3CHROMIUMHelper(
    GLuint program,
    std::vector<int8_t>* result) {
  DCHECK(result);
  helper_->SetBucketSize(kResultBucketId, 0);
  helper_->GetUniformsES3CHROMIUM(program, kResultBucketId);
  GetBucketContents(kResultBucketId, result);
}
