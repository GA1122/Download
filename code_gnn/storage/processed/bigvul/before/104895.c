void PPB_URLLoader_Impl::RegisterCallback(PP_CompletionCallback callback) {
  DCHECK(callback.func);
  DCHECK(!pending_callback_.get() || pending_callback_->completed());

  PP_Resource resource_id = GetReferenceNoAddRef();
  CHECK(resource_id);
  pending_callback_ = new TrackedCompletionCallback(
      instance()->module()->GetCallbackTracker(), resource_id, callback);
}
