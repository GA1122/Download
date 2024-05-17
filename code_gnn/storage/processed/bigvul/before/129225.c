void BackRenderbuffer::Create() {
  ScopedGLErrorSuppressor suppressor("BackRenderbuffer::Create",
                                     state_->GetErrorState());
  Destroy();
  glGenRenderbuffersEXT(1, &id_);
}
