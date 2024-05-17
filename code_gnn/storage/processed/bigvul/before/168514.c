  OnRejected(ScriptState* script_state, ReadableStreamBytesConsumer* consumer)
      : ScriptFunction(script_state), consumer_(consumer) {}
