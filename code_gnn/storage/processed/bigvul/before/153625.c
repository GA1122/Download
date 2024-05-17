void GLES2Implementation::DeleteVertexArraysOESHelper(GLsizei n,
                                                      const GLuint* arrays) {
  vertex_array_object_manager_->DeleteVertexArrays(n, arrays);
  helper_->DeleteVertexArraysOESImmediate(n, arrays);
  IdAllocator* id_allocator = GetIdAllocator(IdNamespaces::kVertexArrays);
  for (GLsizei ii = 0; ii < n; ++ii)
    id_allocator->FreeID(arrays[ii]);
}
