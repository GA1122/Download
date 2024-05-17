static void TraceTopFrame(Isolate* isolate) {
 StackFrameIterator it(isolate);
 if (it.done()) {
 PrintF("unknown location (no JavaScript frames present)");
 return;
 }
 StackFrame* raw_frame = it.frame();
 if (raw_frame->is_internal()) {
 Code* apply_builtin =
        isolate->builtins()->builtin(Builtins::kFunctionPrototypeApply);
 if (raw_frame->unchecked_code() == apply_builtin) {
 PrintF("apply from ");
      it.Advance();
      raw_frame = it.frame();
 }
 }
 JavaScriptFrame::PrintTop(isolate, stdout, false, true);
}
