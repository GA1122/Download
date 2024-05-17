void Document::SendSensitiveInputVisibility() {
  if (sensitive_input_visibility_task_.IsActive())
    return;

  sensitive_input_visibility_task_ = PostCancellableTask(
      *GetTaskRunner(TaskType::kInternalLoading), FROM_HERE,
      WTF::Bind(&Document::SendSensitiveInputVisibilityInternal,
                WrapWeakPersistent(this)));
}
