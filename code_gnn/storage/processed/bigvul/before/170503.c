status_t Parcel::write(const FlattenableHelperInterface& val)
{
 status_t err;

 const size_t len = val.getFlattenedSize();
 const size_t fd_count = val.getFdCount();

    err = this->writeInt32(len);
 if (err) return err;

    err = this->writeInt32(fd_count);
 if (err) return err;

 void* const buf = this->writeInplace(PAD_SIZE(len));
 if (buf == NULL)
 return BAD_VALUE;

 int* fds = NULL;
 if (fd_count) {
        fds = new int[fd_count];
 }

    err = val.flatten(buf, len, fds, fd_count);
 for (size_t i=0 ; i<fd_count && err==NO_ERROR ; i++) {
        err = this->writeDupFileDescriptor( fds[i] );
 }

 if (fd_count) {
 delete [] fds;
 }

 return err;
}
