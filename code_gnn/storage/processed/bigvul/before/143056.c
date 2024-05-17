ScriptPromise BaseAudioContext::decodeAudioData(
    ScriptState* script_state,
    DOMArrayBuffer* audio_data,
    V8DecodeSuccessCallback* success_callback,
    V8DecodeErrorCallback* error_callback,
    ExceptionState& exception_state) {
  DCHECK(IsMainThread());
  DCHECK(audio_data);

  ScriptPromiseResolver* resolver = ScriptPromiseResolver::Create(script_state);
  ScriptPromise promise = resolver->Promise();

  v8::Isolate* isolate = script_state->GetIsolate();
  WTF::ArrayBufferContents buffer_contents;
  if (audio_data->IsNeuterable(isolate) &&
      audio_data->Transfer(isolate, buffer_contents)) {
    DOMArrayBuffer* audio = DOMArrayBuffer::Create(buffer_contents);

    decode_audio_resolvers_.insert(resolver);

    audio_decoder_.DecodeAsync(
        audio, sampleRate(), ToV8PersistentCallbackFunction(success_callback),
        ToV8PersistentCallbackFunction(error_callback), resolver, this);
  } else {
    DOMException* error =
        DOMException::Create(DOMExceptionCode::kDataCloneError,
                             "Cannot decode detached ArrayBuffer");
    resolver->Reject(error);
    if (error_callback) {
      error_callback->InvokeAndReportException(this, error);
    }
  }

  return promise;
}
