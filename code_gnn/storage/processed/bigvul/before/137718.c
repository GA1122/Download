void PrintViewManagerBase::ShouldQuitFromInnerMessageLoop() {
  DCHECK(print_job_->document());
  if (print_job_->document() &&
      print_job_->document()->IsComplete() &&
      inside_inner_message_loop_) {
    base::RunLoop::QuitCurrentWhenIdleDeprecated();
    inside_inner_message_loop_ = false;
  }
}
