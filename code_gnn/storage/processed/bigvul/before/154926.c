bool WebGLRenderingContextBase::DeleteObject(WebGLObject* object) {
  if (isContextLost() || !object)
    return false;
  if (!object->Validate(ContextGroup(), this)) {
    SynthesizeGLError(GL_INVALID_OPERATION, "delete",
                      "object does not belong to this context");
    return false;
  }
  if (object->MarkedForDeletion()) {
    return false;
  }
  if (object->HasObject()) {
    object->DeleteObject(ContextGL());
  }
  return true;
}
