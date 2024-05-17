void GLES2Implementation::DeleteTransformFeedbacksHelper(
    GLsizei n,
    const GLuint* transformfeedbacks) {
  helper_->DeleteTransformFeedbacksImmediate(n, transformfeedbacks);
  IdAllocator* id_allocator = GetIdAllocator(IdNamespaces::kTransformFeedbacks);
  for (GLsizei ii = 0; ii < n; ++ii)
    id_allocator->FreeID(transformfeedbacks[ii]);
}
