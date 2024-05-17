int native_handle_delete(native_handle_t* h)
{
 if (h) {
 if (h->version != sizeof(native_handle_t))
 return -EINVAL;
        free(h);
 }
 return 0;
}
