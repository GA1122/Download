ScriptPromise WebGLRenderingContextBase::commit(
    ScriptState* script_state,
    ExceptionState& exception_state) {
  WebFeature feature = WebFeature::kOffscreenCanvasCommitWebGL;
  UseCounter::Count(ExecutionContext::From(script_state), feature);
  int width = GetDrawingBuffer()->Size().Width();
  int height = GetDrawingBuffer()->Size().Height();
  if (!GetDrawingBuffer()) {
    bool is_web_gl_software_rendering = false;
    return host()->Commit(nullptr, SkIRect::MakeWH(width, height),
                          is_web_gl_software_rendering, script_state,
                          exception_state);
  }

  RefPtr<StaticBitmapImage> image;
  if (CreationAttributes().preserveDrawingBuffer()) {
    SkImageInfo image_info =
        SkImageInfo::Make(width, height, kRGBA_8888_SkColorType,
                          CreationAttributes().alpha() ? kPremul_SkAlphaType
                                                       : kOpaque_SkAlphaType);
    image = StaticBitmapImage::Create(MakeImageSnapshot(image_info));
  } else {
    image = GetDrawingBuffer()->TransferToStaticBitmapImage();
  }

  return host()->Commit(
      std::move(image), SkIRect::MakeWH(width, height),
      GetDrawingBuffer()->ContextProvider()->IsSoftwareRendering(),
      script_state, exception_state);
}
