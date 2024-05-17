base::Closure RunLoop::QuitWhenIdleClosure() {

  return base::Bind(
      &ProxyToTaskRunner, origin_task_runner_,
      base::Bind(&RunLoop::QuitWhenIdle, weak_factory_.GetWeakPtr()));
}
