void PlatformSensorProviderLinux::SetFileTaskRunnerForTesting(
    scoped_refptr<base::SingleThreadTaskRunner> task_runner) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  file_task_runner_ = std::move(task_runner);
}
