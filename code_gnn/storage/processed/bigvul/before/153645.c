void GLES2Implementation::FreeSharedMemory(void* mem) {
  mapped_memory_->FreePendingToken(mem, helper_->InsertToken());
}
