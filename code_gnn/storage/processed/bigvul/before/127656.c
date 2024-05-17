TransportDIB* TransportDIB::CreateWithHandle(Handle shmkey) {
  TransportDIB* dib = new TransportDIB;
  dib->key_.shmkey = shmkey;
  return dib;
}
