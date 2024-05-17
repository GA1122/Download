DEFINE_TRACE(WebGLRenderingContextBase) {
  visitor->Trace(context_group_);
  visitor->Trace(bound_array_buffer_);
  visitor->Trace(default_vertex_array_object_);
  visitor->Trace(bound_vertex_array_object_);
  visitor->Trace(current_program_);
  visitor->Trace(framebuffer_binding_);
  visitor->Trace(renderbuffer_binding_);
  visitor->Trace(texture_units_);
  visitor->Trace(extensions_);
  CanvasRenderingContext::Trace(visitor);
}
