bool WasCanceled(const internal::RasterWorkerPoolTask* task) {
  return task->WasCanceled();
}
