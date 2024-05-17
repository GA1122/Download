bool GLES2Decoder::IsAngle() {
#if defined(OS_WIN)
  return testing_force_is_angle_ ||
         gfx::GetGLImplementation() == gfx::kGLImplementationEGLGLES2;
#else
  return testing_force_is_angle_;
#endif
}
