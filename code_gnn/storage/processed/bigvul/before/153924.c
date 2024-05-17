  TransformFeedback* CreateTransformFeedback(
      GLuint client_id, GLuint service_id) {
    return transform_feedback_manager_->CreateTransformFeedback(
        client_id, service_id);
  }
