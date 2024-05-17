status_t Parcel::read(FlattenableHelperInterface& val) const
{
 const size_t len = this->readInt32();
 const size_t fd_count = this->readInt32();

 if (len > INT32_MAX) {
 return BAD_VALUE;
 }

 void const* const buf = this->readInplace(pad_size(len));
 if (buf == NULL)
 return BAD_VALUE;

 int* fds = NULL;
 if (fd_count) {
        fds = new int[fd_count];
 }

 status_t err = NO_ERROR;
 for (size_t i=0 ; i<fd_count && err==NO_ERROR ; i++) {
        fds[i] = dup(this->readFileDescriptor());
 if (fds[i] < 0) {
            err = BAD_VALUE;
            ALOGE("dup() failed in Parcel::read, i is %zu, fds[i] is %d, fd_count is %zu, error: %s",
                i, fds[i], fd_count, strerror(errno));
 }
 }

 if (err == NO_ERROR) {
        err = val.unflatten(buf, len, fds, fd_count);
 }

 if (fd_count) {
 delete [] fds;
 }

 return err;
}
