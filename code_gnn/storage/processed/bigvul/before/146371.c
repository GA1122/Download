void WebGLRenderingContextBase::SetBoundVertexArrayObject(
    WebGLVertexArrayObjectBase* array_object) {
  if (array_object)
    bound_vertex_array_object_ = array_object;
  else
    bound_vertex_array_object_ = default_vertex_array_object_;
}
