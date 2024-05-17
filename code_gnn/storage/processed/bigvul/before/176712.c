int native_handle_close(const native_handle_t* h)
{
 if (h->version != sizeof(native_handle_t))
 return -EINVAL;

 const int numFds = h->numFds;
 int i;
 for (i=0 ; i<numFds ; i++) {
        close(h->data[i]);
 }
 return 0;
}
