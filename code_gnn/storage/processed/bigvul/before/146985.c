WebLocalFrame* WebLocalFrame::FrameForCurrentContext() {
  v8::Local<v8::Context> context =
      v8::Isolate::GetCurrent()->GetCurrentContext();
  if (context.IsEmpty())
    return 0;
  return FrameForContext(context);
}
