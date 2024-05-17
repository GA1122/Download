void WebGLRenderingContextBase::AddToEvictedList(
    WebGLRenderingContextBase* context) {
  static int generation = 0;
  ForciblyEvictedContexts().Set(context, generation++);
}
