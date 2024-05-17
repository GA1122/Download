bool ContextCreationAttribParser::Parse(const std::vector<int32>& attribs) {
  const int32 EGL_ALPHA_SIZE = 0x3021;
  const int32 EGL_BLUE_SIZE = 0x3022;
  const int32 EGL_GREEN_SIZE = 0x3023;
  const int32 EGL_RED_SIZE = 0x3024;
  const int32 EGL_DEPTH_SIZE = 0x3025;
  const int32 EGL_STENCIL_SIZE = 0x3026;
  const int32 EGL_SAMPLES = 0x3031;
  const int32 EGL_SAMPLE_BUFFERS = 0x3032;
  const int32 EGL_NONE = 0x3038;

  for (size_t i = 0; i < attribs.size(); i += 2) {
    const int32 attrib = attribs[i];
    if (i + 1 >= attribs.size()) {
      if (attrib == EGL_NONE)
        return true;

      DLOG(ERROR) << "Missing value after context creation attribute: "
                  << attrib;
      return false;
    }

    const int32 value = attribs[i+1];
    switch (attrib) {
      case EGL_ALPHA_SIZE:
        alpha_size_ = value;
        break;
      case EGL_BLUE_SIZE:
        blue_size_ = value;
        break;
      case EGL_GREEN_SIZE:
        green_size_ = value;
        break;
      case EGL_RED_SIZE:
        red_size_ = value;
        break;
      case EGL_DEPTH_SIZE:
        depth_size_ = value;
        break;
      case EGL_STENCIL_SIZE:
        stencil_size_ = value;
        break;
      case EGL_SAMPLES:
        samples_ = value;
        break;
      case EGL_SAMPLE_BUFFERS:
        sample_buffers_ = value;
        break;
      case EGL_NONE:
        return true;
      default:
        DLOG(ERROR) << "Invalid context creation attribute: " << attrib;
        return false;
    }
  }

  return true;
}
