void BackendIO::RunTask(const base::Closure& task) {
  operation_ = OP_RUN_TASK;
  task_ = task;
}
