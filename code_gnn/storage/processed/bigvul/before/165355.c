DOMStorageContextWrapper::~DOMStorageContextWrapper() {
  DCHECK(!mojo_state_) << "Shutdown should be called before destruction";
  DCHECK(!mojo_session_state_)
      << "Shutdown should be called before destruction";
}
