TransportDIB::~TransportDIB() {
  if (address_ != kInvalidAddress) {
    shmdt(address_);
    address_ = kInvalidAddress;
  }

  if (x_shm_) {
    DCHECK(display_);
    ui::DetachSharedMemory(display_, x_shm_);
  }
}
