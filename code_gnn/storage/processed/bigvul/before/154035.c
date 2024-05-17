bool GLES2DecoderImpl::DoIsTransformFeedback(GLuint client_id) {
  const TransformFeedback* transform_feedback =
      GetTransformFeedback(client_id);
  return transform_feedback && transform_feedback->has_been_bound();
}
