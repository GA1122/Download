void RenderingHelper::Initialize(
    bool suppress_swap_to_display,
    int num_windows,
    int width,
    int height,
    base::WaitableEvent* done) {
  if (width_) {
    base::WaitableEvent done(false, false);
    UnInitialize(&done);
    done.Wait();
  }

  suppress_swap_to_display_ = suppress_swap_to_display;
  CHECK_GT(width, 0);
  CHECK_GT(height, 0);
  width_ = width;
  height_ = height;
  message_loop_ = MessageLoop::current();
  CHECK_GT(num_windows, 0);

  PlatformInitialize();

  egl_display_ = PlatformGetDisplay();

  EGLint major;
  EGLint minor;
  CHECK(eglInitialize(egl_display_, &major, &minor)) << eglGetError();
  static EGLint rgba8888[] = {
    EGL_RED_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_BLUE_SIZE, 8,
    EGL_ALPHA_SIZE, 8,
    EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
    EGL_NONE,
  };
  EGLConfig egl_config;
  int num_configs;
  CHECK(eglChooseConfig(egl_display_, rgba8888, &egl_config, 1, &num_configs))
      << eglGetError();
  CHECK_GE(num_configs, 1);
  static EGLint context_attribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
  egl_context_ = eglCreateContext(
      egl_display_, egl_config, EGL_NO_CONTEXT, context_attribs);
  CHECK_NE(egl_context_, EGL_NO_CONTEXT) << eglGetError();

  for (int i = 0; i < num_windows; ++i) {
    int top_left_x = (width + 20) * (i % 4);
    int top_left_y = (height + 12) * (i % 3);

    EGLNativeWindowType window = PlatformCreateWindow(top_left_x, top_left_y);
    EGLSurface egl_surface =
        eglCreateWindowSurface(egl_display_, egl_config, window, NULL);
    egl_surfaces_.push_back(egl_surface);
    CHECK_NE(egl_surface, EGL_NO_SURFACE);
  }
  CHECK(eglMakeCurrent(egl_display_, egl_surfaces_[0],
                       egl_surfaces_[0], egl_context_)) << eglGetError();

  static const float kVertices[] =
      { -1.f, 1.f, -1.f, -1.f, 1.f, 1.f, 1.f, -1.f, };
  static const float kTextureCoordsEgl[] = { 0, 1, 0, 0, 1, 1, 1, 0, };
  static const char kVertexShader[] = STRINGIZE(
      varying vec2 interp_tc;
      attribute vec4 in_pos;
      attribute vec2 in_tc;
      void main() {
        interp_tc = in_tc;
        gl_Position = in_pos;
      }
                                                );
  static const char kFragmentShaderEgl[] = STRINGIZE(
      precision mediump float;
      varying vec2 interp_tc;
      uniform sampler2D tex;
      void main() {
        gl_FragColor = texture2D(tex, interp_tc);
      }
                                                     );
  GLuint program = glCreateProgram();
  CreateShader(program, GL_VERTEX_SHADER,
               kVertexShader, arraysize(kVertexShader));
  CreateShader(program, GL_FRAGMENT_SHADER,
               kFragmentShaderEgl, arraysize(kFragmentShaderEgl));
  glLinkProgram(program);
  int result = GL_FALSE;
  glGetProgramiv(program, GL_LINK_STATUS, &result);
  if (!result) {
    char log[4096];
    glGetShaderInfoLog(program, arraysize(log), NULL, log);
    LOG(FATAL) << log;
  }
  glUseProgram(program);
  glDeleteProgram(program);

  glUniform1i(glGetUniformLocation(program, "tex"), 0);
  int pos_location = glGetAttribLocation(program, "in_pos");
  glEnableVertexAttribArray(pos_location);
  glVertexAttribPointer(pos_location, 2, GL_FLOAT, GL_FALSE, 0, kVertices);
  int tc_location = glGetAttribLocation(program, "in_tc");
  glEnableVertexAttribArray(tc_location);
  glVertexAttribPointer(tc_location, 2, GL_FLOAT, GL_FALSE, 0,
                        kTextureCoordsEgl);
  done->Signal();
}
