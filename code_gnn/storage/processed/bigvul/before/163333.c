RenderThreadImpl::GetIOTaskRunner() {
  return ChildProcess::current()->io_task_runner();
}
