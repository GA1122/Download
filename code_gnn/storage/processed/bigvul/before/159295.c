void WebGLRenderingContextBase::DestroyContext() {
  if (!GetDrawingBuffer())
    return;

  extensions_util_.reset();

  WTF::RepeatingClosure null_closure;
  WTF::RepeatingFunction<void(const char*, int32_t)> null_function;
  GetDrawingBuffer()->ContextProvider()->SetLostContextCallback(
      std::move(null_closure));
  GetDrawingBuffer()->ContextProvider()->SetErrorMessageCallback(
      std::move(null_function));

  DCHECK(GetDrawingBuffer());
  drawing_buffer_->BeginDestruction();
  drawing_buffer_ = nullptr;
}
