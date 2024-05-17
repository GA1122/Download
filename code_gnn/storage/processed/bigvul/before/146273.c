void WebGLRenderingContextBase::DestroyContext() {
  if (!GetDrawingBuffer())
    return;

  extensions_util_.reset();

  std::unique_ptr<WTF::Closure> null_closure;
  std::unique_ptr<WTF::Function<void(const char*, int32_t)>> null_function;
  GetDrawingBuffer()->ContextProvider()->SetLostContextCallback(
      ConvertToBaseCallback(std::move(null_closure)));
  GetDrawingBuffer()->ContextProvider()->SetErrorMessageCallback(
      ConvertToBaseCallback(std::move(null_function)));

  DCHECK(GetDrawingBuffer());
  drawing_buffer_->BeginDestruction();
  drawing_buffer_.Clear();
}
