void SetRecordActionTaskRunner(
    scoped_refptr<SingleThreadTaskRunner> task_runner) {
  DCHECK(task_runner->BelongsToCurrentThread());
  DCHECK(!g_task_runner.Get() || g_task_runner.Get()->BelongsToCurrentThread());
  g_task_runner.Get() = task_runner;
}
