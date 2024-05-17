void WebGL2RenderingContextBase::deleteQuery(WebGLQuery* query) {
  if (isContextLost() || !query)
    return;

  if (current_boolean_occlusion_query_ == query) {
    ContextGL()->EndQueryEXT(current_boolean_occlusion_query_->GetTarget());
    current_boolean_occlusion_query_ = nullptr;
  }

  if (current_transform_feedback_primitives_written_query_ == query) {
    ContextGL()->EndQueryEXT(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
    current_transform_feedback_primitives_written_query_ = nullptr;
  }

  if (current_elapsed_query_ == query) {
    ContextGL()->EndQueryEXT(current_elapsed_query_->GetTarget());
    current_elapsed_query_ = nullptr;
  }

  DeleteObject(query);
}
