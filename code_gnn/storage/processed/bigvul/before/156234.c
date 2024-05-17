void RendererSchedulerImpl::OnTaskCompleted(
    MainThreadTaskQueue* queue,
    const TaskQueue::Task& task,
    base::TimeTicks start,
    base::TimeTicks end,
    base::Optional<base::TimeDelta> thread_time) {
  DCHECK_LE(start, end);
  seqlock_queueing_time_estimator_.seqlock.WriteBegin();
  seqlock_queueing_time_estimator_.data.OnTopLevelTaskCompleted(end);
  seqlock_queueing_time_estimator_.seqlock.WriteEnd();

  task_queue_throttler()->OnTaskRunTimeReported(queue, start, end);

  main_thread_only().metrics_helper.RecordTaskMetrics(queue, task, start, end,
                                                      thread_time);
  main_thread_only().task_description_for_tracing = base::nullopt;
}
