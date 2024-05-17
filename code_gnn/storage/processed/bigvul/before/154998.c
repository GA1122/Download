bool WebGLRenderingContextBase::ValidateNullableWebGLObject(
    const char* function_name,
    WebGLObject* object) {
  if (isContextLost())
    return false;
  if (!object) {
    return true;
  }
  return ValidateWebGLObject(function_name, object);
}
