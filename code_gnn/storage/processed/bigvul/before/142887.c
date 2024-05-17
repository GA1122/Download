void HTMLMediaElement::ScheduleRejectPlayPromises(DOMExceptionCode code) {
  DCHECK(play_promise_reject_list_.IsEmpty() ||
         play_promise_reject_task_handle_.IsActive());
  if (play_promise_resolvers_.IsEmpty())
    return;

  play_promise_reject_list_.AppendVector(play_promise_resolvers_);
  play_promise_resolvers_.clear();

  if (play_promise_reject_task_handle_.IsActive())
    return;

  play_promise_error_code_ = code;
  play_promise_reject_task_handle_ = PostCancellableTask(
      *GetDocument().GetTaskRunner(TaskType::kMediaElementEvent), FROM_HERE,
      WTF::Bind(&HTMLMediaElement::RejectScheduledPlayPromises,
                WrapWeakPersistent(this)));
}
