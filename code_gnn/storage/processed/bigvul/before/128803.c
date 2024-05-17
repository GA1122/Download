ui::EglConfigCallbacks GetEglConfigCallbacks(EGLDisplay display) {
  ui::EglConfigCallbacks callbacks;
  callbacks.choose_config = base::Bind(EglChooseConfig, display);
  callbacks.get_config_attribute = base::Bind(EglGetConfigAttribute, display);
  callbacks.get_last_error_string = base::Bind(&ui::GetLastEGLErrorString);
  return callbacks;
}
