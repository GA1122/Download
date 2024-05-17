bool WebGLRenderingContextBase::CheckObjectToBeBound(const char* function_name,
                                                     WebGLObject* object,
                                                     bool& deleted) {
  deleted = false;
  if (isContextLost())
    return false;
  if (object) {
    if (!object->Validate(ContextGroup(), this)) {
      SynthesizeGLError(GL_INVALID_OPERATION, function_name,
                        "object not from this context");
      return false;
    }
    deleted = !object->HasObject();
  }
  return true;
}
