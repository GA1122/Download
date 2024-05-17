bool TransportDIB::Map() {
  if (!is_valid_id(key_))
    return false;
  if (address_ != kInvalidAddress)
    return true;

  struct shmid_ds shmst;
  if (shmctl(key_.shmkey, IPC_STAT, &shmst) == -1)
    return false;

  void* address = shmat(key_.shmkey, NULL  , 0  );
  if (address == kInvalidAddress)
    return false;

  address_ = address;
  size_ = shmst.shm_segsz;
  return true;
}
