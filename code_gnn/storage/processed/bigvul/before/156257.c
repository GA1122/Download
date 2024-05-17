void RendererSchedulerImpl::RunIdleTasksForTesting(
    const base::Closure& callback) {
  main_thread_only().in_idle_period_for_testing = true;
  IdleTaskRunner()->PostIdleTask(
      FROM_HERE, base::Bind(&RendererSchedulerImpl::EndIdlePeriodForTesting,
                            weak_factory_.GetWeakPtr(), callback));
  idle_helper_.EnableLongIdlePeriod();
}
