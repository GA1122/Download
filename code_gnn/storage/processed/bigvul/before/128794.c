bool EglChooseConfig(EGLDisplay display,
                     const int32_t* attribs,
                     EGLConfig* configs,
                     int32_t config_size,
                     int32_t* num_configs) {
  return eglChooseConfig(display, attribs, configs, config_size, num_configs);
}
