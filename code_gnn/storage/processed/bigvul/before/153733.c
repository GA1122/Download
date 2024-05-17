void GLES2Implementation::GetUniformBlocksCHROMIUMHelper(
    GLuint program,
    std::vector<int8_t>* result) {
  DCHECK(result);
  helper_->SetBucketSize(kResultBucketId, 0);
  helper_->GetUniformBlocksCHROMIUM(program, kResultBucketId);
  GetBucketContents(kResultBucketId, result);
}
