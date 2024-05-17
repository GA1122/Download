status_t Parcel::writeNativeHandle(const native_handle* handle)
{
 if (!handle || handle->version != sizeof(native_handle))
 return BAD_TYPE;

 status_t err;
    err = writeInt32(handle->numFds);
 if (err != NO_ERROR) return err;

    err = writeInt32(handle->numInts);
 if (err != NO_ERROR) return err;

 for (int i=0 ; err==NO_ERROR && i<handle->numFds ; i++)
        err = writeDupFileDescriptor(handle->data[i]);

 if (err != NO_ERROR) {
        ALOGD("write native handle, write dup fd failed");
 return err;
 }
    err = write(handle->data + handle->numFds, sizeof(int)*handle->numInts);
 return err;
}
