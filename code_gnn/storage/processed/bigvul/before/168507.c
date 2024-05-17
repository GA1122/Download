  static v8::Local<v8::Function> CreateFunction(
      ScriptState* script_state,
      ReadableStreamBytesConsumer* consumer) {
    return (new OnRejected(script_state, consumer))->BindToV8Function();
  }
