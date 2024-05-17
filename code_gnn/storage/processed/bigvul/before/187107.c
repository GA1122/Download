  void WebGL2RenderingContextBase::bindVertexArray(
      WebGLVertexArrayObject* vertex_array) {
  if (isContextLost())
//   bool deleted;
//   if (!CheckObjectToBeBound("bindVertexArray", vertex_array, deleted))
      return;
  if (vertex_array &&
      (vertex_array->IsDeleted() || !vertex_array->Validate(nullptr, this))) {
//   if (deleted) {
      SynthesizeGLError(GL_INVALID_OPERATION, "bindVertexArray",
                      "invalid vertexArray");
//                       "attempt to bind a deleted vertex array");
      return;
    }
  
   if (vertex_array && !vertex_array->IsDefaultObject() &&
       vertex_array->Object()) {
     ContextGL()->BindVertexArrayOES(ObjectOrZero(vertex_array));
 
     vertex_array->SetHasEverBeenBound();
     SetBoundVertexArrayObject(vertex_array);
   } else {
     ContextGL()->BindVertexArrayOES(0);
     SetBoundVertexArrayObject(nullptr);
   }
 }