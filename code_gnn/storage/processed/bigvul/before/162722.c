CanvasPattern* BaseRenderingContext2D::createPattern(
    ScriptState* script_state,
    const CanvasImageSourceUnion& image_source,
    const String& repetition_type,
    ExceptionState& exception_state) {
  CanvasImageSource* image_source_internal =
      ToImageSourceInternal(image_source, exception_state);
  if (!image_source_internal) {
    return nullptr;
  }

  return createPattern(script_state, image_source_internal, repetition_type,
                       exception_state);
}
