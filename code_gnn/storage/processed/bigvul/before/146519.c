long long WebGLRenderingContextBase::getVertexAttribOffset(GLuint index,
                                                           GLenum pname) {
  if (isContextLost())
    return 0;
  GLvoid* result = nullptr;
  ContextGL()->GetVertexAttribPointerv(index, pname, &result);
  return static_cast<long long>(reinterpret_cast<intptr_t>(result));
}
