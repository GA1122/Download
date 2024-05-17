const char* GetGLImplName(gl::GLImplementation impl) {
  switch (impl) {
    case gl::kGLImplementationDesktopGL:
      return gl::kGLImplementationDesktopName;
    case gl::kGLImplementationOSMesaGL:
      return gl::kGLImplementationOSMesaName;
    case gl::kGLImplementationEGLGLES2:
      return gl::kGLImplementationEGLName;
    default:
      return "unknown";
  }
}
