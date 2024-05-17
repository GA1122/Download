void RenderThreadImpl::OnRAILModeChanged(v8::RAILMode rail_mode) {
  blink::MainThreadIsolate()->SetRAILMode(rail_mode);
  blink::SetRAILModeOnWorkerThreadIsolates(rail_mode);
}
