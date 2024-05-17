void GLSurfaceOzoneSurfaceless::FenceRetired(EGLSyncKHR fence,
                                             PendingFrame* frame) {
  eglDestroySyncKHR(GetDisplay(), fence);
  frame->ready = true;
  SubmitFrame();
}
