static bool IsAngle() {
#if defined(OS_WIN)
  return gfx::GetGLImplementation() == gfx::kGLImplementationEGLGLES2;
#else
  return false;
#endif
}
