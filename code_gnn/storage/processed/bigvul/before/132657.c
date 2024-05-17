void BlinkTestRunner::PostDelayedTask(WebTask* task, long long ms) {
  Platform::current()->currentThread()->postDelayedTask(
      WebTraceLocation(__FUNCTION__, __FILE__),
      new InvokeTaskHelper(make_scoped_ptr(task)), ms);
}
