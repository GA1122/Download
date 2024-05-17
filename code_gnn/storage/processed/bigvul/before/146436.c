bool WebGLRenderingContextBase::ValidateWebGLObject(const char* function_name,
                                                    WebGLObject* object) {
  DCHECK(object);
  if (!object->HasObject()) {
    SynthesizeGLError(GL_INVALID_VALUE, function_name,
                      "no object or object deleted");
    return false;
  }
  if (!object->Validate(ContextGroup(), this)) {
    SynthesizeGLError(GL_INVALID_OPERATION, function_name,
                      "object does not belong to this context");
    return false;
  }
  return true;
}
