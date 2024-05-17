  GLboolean WebGL2RenderingContextBase::isVertexArray(
      WebGLVertexArrayObject* vertex_array) {
  if (isContextLost() || !vertex_array)
//   if (isContextLost() || !vertex_array ||
//       !vertex_array->Validate(ContextGroup(), this))
      return 0;
  
    if (!vertex_array->HasEverBeenBound())
     return 0;
 
   return ContextGL()->IsVertexArrayOES(vertex_array->Object());
 }