XID TransportDIB::MapToX(Display* display) {
  if (!x_shm_) {
    x_shm_ = ui::AttachSharedMemory(display, key_.shmkey);
    display_ = display;
  }

  return x_shm_;
}
