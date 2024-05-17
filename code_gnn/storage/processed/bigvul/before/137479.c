bool RunLoop::Delegate::Client::IsNested() const {
  DCHECK_CALLED_ON_VALID_THREAD(outer_->bound_thread_checker_);
  DCHECK(outer_->bound_);
   return outer_->active_run_loops_.size() > 1;
 }
