void GLES2DecoderImpl::GetIndexedIntegerImpl(
    const char* function_name, GLenum target, GLuint index, TYPE* data) {
  DCHECK(data);

  if (features().ext_window_rectangles && target == GL_WINDOW_RECTANGLE_EXT) {
    if (index >= state_.GetMaxWindowRectangles()) {
      LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, function_name,
                         "window rectangle index out of bounds");
    }
    state_.GetWindowRectangle(index, data);
    return;
  }
  scoped_refptr<IndexedBufferBindingHost> bindings;
  switch (target) {
    case GL_TRANSFORM_FEEDBACK_BUFFER_BINDING:
    case GL_TRANSFORM_FEEDBACK_BUFFER_SIZE:
    case GL_TRANSFORM_FEEDBACK_BUFFER_START:
      if (index >= group_->max_transform_feedback_separate_attribs()) {
        LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, function_name, "invalid index");
        return;
      }
      bindings = state_.bound_transform_feedback.get();
      break;
    case GL_UNIFORM_BUFFER_BINDING:
    case GL_UNIFORM_BUFFER_SIZE:
    case GL_UNIFORM_BUFFER_START:
      if (index >= group_->max_uniform_buffer_bindings()) {
        LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, function_name, "invalid index");
        return;
      }
      bindings = state_.indexed_uniform_buffer_bindings.get();
      break;
    default:
      NOTREACHED();
      break;
  }
  DCHECK(bindings);
  switch (target) {
    case GL_TRANSFORM_FEEDBACK_BUFFER_BINDING:
    case GL_UNIFORM_BUFFER_BINDING:
      {
        Buffer* buffer = bindings->GetBufferBinding(index);
        *data = static_cast<TYPE>(buffer ? buffer->service_id() : 0);
      }
      break;
    case GL_TRANSFORM_FEEDBACK_BUFFER_SIZE:
    case GL_UNIFORM_BUFFER_SIZE:
      *data = static_cast<TYPE>(bindings->GetBufferSize(index));
      break;
    case GL_TRANSFORM_FEEDBACK_BUFFER_START:
    case GL_UNIFORM_BUFFER_START:
      *data = static_cast<TYPE>(bindings->GetBufferStart(index));
      break;
    default:
      NOTREACHED();
      break;
  }
}
