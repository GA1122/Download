ScriptPromise ImageBitmapFactories::CreateImageBitmap(
    ScriptState* script_state,
    EventTarget& event_target,
    ImageBitmapSource* bitmap_source,
    base::Optional<IntRect> crop_rect,
    const ImageBitmapOptions* options) {
  if (crop_rect && (crop_rect->Width() == 0 || crop_rect->Height() == 0)) {
    return ScriptPromise::Reject(
        script_state,
        V8ThrowException::CreateRangeError(
            script_state->GetIsolate(),
            String::Format("The crop rect %s is 0.",
                           crop_rect->Width() ? "height" : "width")));
  }

  if (bitmap_source->IsBlob()) {
    return CreateImageBitmapFromBlob(script_state, event_target, bitmap_source,
                                     crop_rect, options);
  }

  if (bitmap_source->BitmapSourceSize().Width() == 0 ||
      bitmap_source->BitmapSourceSize().Height() == 0) {
    return ScriptPromise::RejectWithDOMException(
        script_state,
        DOMException::Create(
            DOMExceptionCode::kInvalidStateError,
            String::Format("The source image %s is 0.",
                           bitmap_source->BitmapSourceSize().Width()
                               ? "height"
                               : "width")));
  }

  return bitmap_source->CreateImageBitmap(script_state, event_target, crop_rect,
                                          options);
}
