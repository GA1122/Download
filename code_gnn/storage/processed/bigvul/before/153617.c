bool GLES2Implementation::DeleteShaderHelper(GLuint shader) {
  if (!GetIdHandler(SharedIdNamespaces::kProgramsAndShaders)
           ->FreeIds(this, 1, &shader,
                     &GLES2Implementation::DeleteShaderStub)) {
    SetGLError(GL_INVALID_VALUE, "glDeleteShader",
               "id not created by this context.");
    return false;
  }
  return true;
}
