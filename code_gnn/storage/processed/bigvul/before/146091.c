void WebGL2RenderingContextBase::endQuery(GLenum target) {
  if (isContextLost())
    return;

  switch (target) {
    case GL_ANY_SAMPLES_PASSED:
    case GL_ANY_SAMPLES_PASSED_CONSERVATIVE: {
      if (current_boolean_occlusion_query_ &&
          current_boolean_occlusion_query_->GetTarget() == target) {
        current_boolean_occlusion_query_->ResetCachedResult();
        current_boolean_occlusion_query_ = nullptr;
      } else {
        SynthesizeGLError(GL_INVALID_OPERATION, "endQuery",
                          "target query is not active");
        return;
      }
    } break;
    case GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN: {
      if (current_transform_feedback_primitives_written_query_) {
        current_transform_feedback_primitives_written_query_
            ->ResetCachedResult();
        current_transform_feedback_primitives_written_query_ = nullptr;
      } else {
        SynthesizeGLError(GL_INVALID_OPERATION, "endQuery",
                          "target query is not active");
        return;
      }
    } break;
    case GL_TIME_ELAPSED_EXT: {
      if (!ExtensionEnabled(kEXTDisjointTimerQueryWebGL2Name)) {
        SynthesizeGLError(GL_INVALID_ENUM, "endQuery", "invalid target");
        return;
      }
      if (current_elapsed_query_) {
        current_elapsed_query_->ResetCachedResult();
        current_elapsed_query_ = nullptr;
      } else {
        SynthesizeGLError(GL_INVALID_OPERATION, "endQuery",
                          "target query is not active");
        return;
      }
    } break;
    default:
      SynthesizeGLError(GL_INVALID_ENUM, "endQuery", "invalid target");
      return;
  }

  ContextGL()->EndQueryEXT(target);
}
