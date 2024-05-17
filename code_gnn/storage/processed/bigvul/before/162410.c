MojoResult Core::GetProperty(MojoPropertyType type, void* value) {
  base::AutoLock locker(property_lock_);
  switch (type) {
    case MOJO_PROPERTY_TYPE_SYNC_CALL_ALLOWED:
      *static_cast<bool*>(value) = property_sync_call_allowed_;
      return MOJO_RESULT_OK;
    default:
      return MOJO_RESULT_INVALID_ARGUMENT;
  }
}
