ScriptPromise ImageBitmapFactories::CreateImageBitmap(
    ScriptState* script_state,
    EventTarget& event_target,
    const ImageBitmapSourceUnion& bitmap_source,
    const ImageBitmapOptions* options) {
  WebFeature feature = WebFeature::kCreateImageBitmap;
  UseCounter::Count(ExecutionContext::From(script_state), feature);
  ImageBitmapSource* bitmap_source_internal =
      ToImageBitmapSourceInternal(bitmap_source, options, false);
  if (!bitmap_source_internal)
    return ScriptPromise();
  return CreateImageBitmap(script_state, event_target, bitmap_source_internal,
                           base::Optional<IntRect>(), options);
}
