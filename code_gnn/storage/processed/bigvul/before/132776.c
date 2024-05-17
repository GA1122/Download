void PepperVideoRenderer3D::EnsureProgramForTexture(uint32_t texture_target) {
  static const char kVertexShader[] =
      "varying vec2 v_texCoord;            \n"
      "attribute vec4 a_position;          \n"
      "attribute vec2 a_texCoord;          \n"
      "uniform vec2 v_scale;               \n"
      "void main()                         \n"
      "{                                   \n"
      "    v_texCoord = v_scale * a_texCoord; \n"
      "    gl_Position = a_position;       \n"
      "}";

  static const char kFragmentShader2D[] =
      "precision mediump float;            \n"
      "varying vec2 v_texCoord;            \n"
      "uniform sampler2D s_texture;        \n"
      "void main()                         \n"
      "{"
      "    gl_FragColor = texture2D(s_texture, v_texCoord); \n"
      "}";

  static const char kFragmentShaderRectangle[] =
      "#extension GL_ARB_texture_rectangle : require\n"
      "precision mediump float;            \n"
      "varying vec2 v_texCoord;            \n"
      "uniform sampler2DRect s_texture;    \n"
      "void main()                         \n"
      "{"
      "    gl_FragColor = texture2DRect(s_texture, v_texCoord).rgba; \n"
      "}";

  static const char kFragmentShaderExternal[] =
      "#extension GL_OES_EGL_image_external : require\n"
      "precision mediump float;            \n"
      "varying vec2 v_texCoord;            \n"
      "uniform samplerExternalOES s_texture; \n"
      "void main()                         \n"
      "{"
      "    gl_FragColor = texture2D(s_texture, v_texCoord); \n"
      "}";

  if (current_shader_program_texture_target_ != texture_target) {
    current_shader_program_texture_target_ = texture_target;

    if (texture_target == GL_TEXTURE_2D) {
      CreateProgram(kVertexShader, kFragmentShader2D);
    } else if (texture_target == GL_TEXTURE_RECTANGLE_ARB) {
      CreateProgram(kVertexShader, kFragmentShaderRectangle);
    } else if (texture_target == GL_TEXTURE_EXTERNAL_OES) {
      CreateProgram(kVertexShader, kFragmentShaderExternal);
    } else {
      LOG(FATAL) << "Unknown texture target: " << texture_target;
    }
  }
}
