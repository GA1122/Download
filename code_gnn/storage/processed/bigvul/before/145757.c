v8::Local<v8::String> ModuleSystem::WrapSource(v8::Local<v8::String> source) {
  v8::EscapableHandleScope handle_scope(GetIsolate());
  v8::Local<v8::String> left = ToV8StringUnsafe(
      GetIsolate(),
      "(function(define, require, requireNative, requireAsync, exports, "
      "console, privates,"
      "$Array, $Function, $JSON, $Object, $RegExp, $String, $Error) {"
      "'use strict';");
  v8::Local<v8::String> right = ToV8StringUnsafe(GetIsolate(), "\n})");
  return handle_scope.Escape(v8::Local<v8::String>(
      v8::String::Concat(left, v8::String::Concat(source, right))));
}
