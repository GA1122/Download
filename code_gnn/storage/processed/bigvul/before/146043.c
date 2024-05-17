void WebGL2RenderingContextBase::beginQuery(GLenum target, WebGLQuery* query) {
  bool deleted;
  DCHECK(query);
  if (!CheckObjectToBeBound("beginQuery", query, deleted))
    return;
  if (deleted) {
    SynthesizeGLError(GL_INVALID_OPERATION, "beginQuery",
                      "attempted to begin a deleted query object");
    return;
  }

  if (query->GetTarget() && query->GetTarget() != target) {
    SynthesizeGLError(GL_INVALID_OPERATION, "beginQuery",
                      "query type does not match target");
    return;
  }

  switch (target) {
    case GL_ANY_SAMPLES_PASSED:
    case GL_ANY_SAMPLES_PASSED_CONSERVATIVE: {
      if (current_boolean_occlusion_query_) {
        SynthesizeGLError(GL_INVALID_OPERATION, "beginQuery",
                          "a query is already active for target");
        return;
      }
      current_boolean_occlusion_query_ = query;
    } break;
    case GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN: {
      if (current_transform_feedback_primitives_written_query_) {
        SynthesizeGLError(GL_INVALID_OPERATION, "beginQuery",
                          "a query is already active for target");
        return;
      }
      current_transform_feedback_primitives_written_query_ = query;
    } break;
    case GL_TIME_ELAPSED_EXT: {
      if (!ExtensionEnabled(kEXTDisjointTimerQueryWebGL2Name)) {
        SynthesizeGLError(GL_INVALID_ENUM, "beginQuery", "invalid target");
        return;
      }
      if (current_elapsed_query_) {
        SynthesizeGLError(GL_INVALID_OPERATION, "beginQuery",
                          "a query is already active for target");
        return;
      }
      current_elapsed_query_ = query;
    } break;
    default:
      SynthesizeGLError(GL_INVALID_ENUM, "beginQuery", "invalid target");
      return;
  }

  if (!query->GetTarget())
    query->SetTarget(target);

  ContextGL()->BeginQueryEXT(target, query->Object());
}
