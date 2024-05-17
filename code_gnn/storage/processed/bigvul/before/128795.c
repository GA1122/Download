bool EglGetConfigAttribute(EGLDisplay display,
                           EGLConfig config,
                           int32_t attribute,
                           int32_t* value) {
  return eglGetConfigAttrib(display, config, attribute, value);
}
