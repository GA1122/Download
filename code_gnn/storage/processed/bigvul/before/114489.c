void RenderingHelper::UnInitialize(base::WaitableEvent* done) {
  CHECK_EQ(MessageLoop::current(), message_loop_);
  CHECK(eglMakeCurrent(egl_display_, EGL_NO_SURFACE, EGL_NO_SURFACE,
                       EGL_NO_CONTEXT)) << eglGetError();
  CHECK(eglDestroyContext(egl_display_, egl_context_));
  for (size_t i = 0; i < egl_surfaces_.size(); ++i)
    CHECK(eglDestroySurface(egl_display_, egl_surfaces_[i]));
  CHECK(eglTerminate(egl_display_));
  Clear();
  done->Signal();
}
