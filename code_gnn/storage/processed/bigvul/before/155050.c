int64_t WebGLRenderingContextBase::getVertexAttribOffset(GLuint index,
                                                         GLenum pname) {
  if (isContextLost())
    return 0;
  GLvoid* result = nullptr;
  ContextGL()->GetVertexAttribPointerv(index, pname, &result);
  return static_cast<int64_t>(reinterpret_cast<intptr_t>(result));
}
