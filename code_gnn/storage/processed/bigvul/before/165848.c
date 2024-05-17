ScriptPromise ImageBitmapFactories::CreateImageBitmap(
    ScriptState* script_state,
    EventTarget& event_target,
    const ImageBitmapSourceUnion& bitmap_source,
    int sx,
    int sy,
    int sw,
    int sh,
    const ImageBitmapOptions* options) {
  WebFeature feature = WebFeature::kCreateImageBitmap;
  UseCounter::Count(ExecutionContext::From(script_state), feature);
  ImageBitmapSource* bitmap_source_internal =
      ToImageBitmapSourceInternal(bitmap_source, options, true);
  if (!bitmap_source_internal)
    return ScriptPromise();
  base::Optional<IntRect> crop_rect = IntRect(sx, sy, sw, sh);
  return CreateImageBitmap(script_state, event_target, bitmap_source_internal,
                           crop_rect, options);
}
