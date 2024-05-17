ScriptPromise ImageLoader::Decode(ScriptState* script_state,
                                  ExceptionState& exception_state) {
  if (!script_state->ContextIsValid()) {
    exception_state.ThrowDOMException(kEncodingError,
                                      "The source image cannot be decoded.");
    return ScriptPromise();
  }

  UseCounter::Count(GetElement()->GetDocument(), WebFeature::kImageDecodeAPI);

  auto* request =
      new DecodeRequest(this, ScriptPromiseResolver::Create(script_state));
  Microtask::EnqueueMicrotask(
      WTF::Bind(&DecodeRequest::ProcessForTask, WrapWeakPersistent(request)));
  decode_requests_.push_back(request);
  return request->promise();
}
