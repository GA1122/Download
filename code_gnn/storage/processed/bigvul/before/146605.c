WebGLRenderingContextBase::~WebGLRenderingContextBase() {
  DestroyContext();

  RestoreEvictedContext(this);
}
