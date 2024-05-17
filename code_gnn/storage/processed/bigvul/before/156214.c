bool RendererSchedulerImpl::IsHighPriorityWorkAnticipated() {
  helper_.CheckOnValidThread();
  if (helper_.IsShutdown())
    return false;

  MaybeUpdatePolicy();
  UseCase use_case = main_thread_only().current_use_case;
  return main_thread_only().touchstart_expected_soon ||
         use_case == UseCase::kTouchstart ||
         use_case == UseCase::kMainThreadGesture ||
         use_case == UseCase::kMainThreadCustomInputHandling ||
         use_case == UseCase::kSynchronizedGesture;
}
