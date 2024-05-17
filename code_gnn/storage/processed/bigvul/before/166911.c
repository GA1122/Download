void Performance::mark(ScriptState* script_state,
                       const String& mark_name,
                       ExceptionState& exception_state) {
  DoubleOrPerformanceMarkOptions startOrOptions;
  this->mark(script_state, mark_name, startOrOptions, exception_state);
}
