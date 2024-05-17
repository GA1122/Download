void ContextState::RestoreVertexAttribValues() const {
  for (size_t attrib = 0; attrib < vertex_attrib_manager->num_attribs();
       ++attrib) {
    switch (attrib_values[attrib].type()) {
      case SHADER_VARIABLE_FLOAT:
        {
          GLfloat v[4];
          attrib_values[attrib].GetValues(v);
          api()->glVertexAttrib4fvFn(attrib, v);
        }
        break;
      case SHADER_VARIABLE_INT:
        {
          GLint v[4];
          attrib_values[attrib].GetValues(v);
          api()->glVertexAttribI4ivFn(attrib, v);
        }
        break;
      case SHADER_VARIABLE_UINT:
        {
          GLuint v[4];
          attrib_values[attrib].GetValues(v);
          api()->glVertexAttribI4uivFn(attrib, v);
        }
        break;
      default:
        NOTREACHED();
        break;
    }
  }
}
