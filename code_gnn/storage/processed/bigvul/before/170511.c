status_t Parcel::writeFileDescriptor(int fd, bool takeOwnership)
{
    flat_binder_object obj;
    obj.type = BINDER_TYPE_FD;
    obj.flags = 0x7f | FLAT_BINDER_FLAG_ACCEPTS_FDS;
    obj.binder = 0;  
    obj.handle = fd;
    obj.cookie = takeOwnership ? 1 : 0;
 return writeObject(obj, true);
}
