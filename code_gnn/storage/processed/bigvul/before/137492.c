void RunLoop::Run() {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);

  if (!BeforeRun())
    return;

  DETACH_FROM_SEQUENCE(sequence_checker_);

  tracked_objects::TaskStopwatch stopwatch;
  stopwatch.Start();
  delegate_->Run();
  stopwatch.Stop();

  DETACH_FROM_SEQUENCE(sequence_checker_);
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);

  AfterRun();
}
