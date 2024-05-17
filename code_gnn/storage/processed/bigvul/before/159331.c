ScriptPromise WebGLRenderingContextBase::commit(
    ScriptState* script_state,
    ExceptionState& exception_state) {
  WebFeature feature = WebFeature::kOffscreenCanvasCommitWebGL;
  UseCounter::Count(ExecutionContext::From(script_state), feature);
  int width = GetDrawingBuffer()->Size().Width();
  int height = GetDrawingBuffer()->Size().Height();
  if (!GetDrawingBuffer()) {
    return Host()->Commit(nullptr, SkIRect::MakeWH(width, height), script_state,
                          exception_state);
  }

  scoped_refptr<StaticBitmapImage> image = GetStaticBitmapImage();

  return Host()->Commit(
      std::move(image), SkIRect::MakeWH(width, height),
      script_state, exception_state);
}
