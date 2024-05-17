static LocalFrame* FrameForExecutionContext(ExecutionContext* context) {
  LocalFrame* frame = nullptr;
  if (context->IsDocument())
    frame = ToDocument(context)->GetFrame();
  return frame;
}
