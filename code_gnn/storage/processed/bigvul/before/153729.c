void GLES2Implementation::GetTransformFeedbackVaryingsCHROMIUMHelper(
    GLuint program,
    std::vector<int8_t>* result) {
  DCHECK(result);
  helper_->SetBucketSize(kResultBucketId, 0);
  helper_->GetTransformFeedbackVaryingsCHROMIUM(program, kResultBucketId);
  GetBucketContents(kResultBucketId, result);
}
