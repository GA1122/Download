void RenderThreadImpl::OnTrimMemoryImmediately() {
  if (blink::MainThreadIsolate()) {
    blink::MainThreadIsolate()->MemoryPressureNotification(
        v8::MemoryPressureLevel::kCritical);
    blink::MemoryPressureNotificationToWorkerThreadIsolates(
        v8::MemoryPressureLevel::kCritical);
  }
}
