status_t Camera3Device::pushReprocessBuffer(int reprocessStreamId,
 buffer_handle_t *buffer, wp<BufferReleasedListener> listener) {
    ATRACE_CALL();
 (void)reprocessStreamId; (void)buffer; (void)listener;

    CLOGE("Unimplemented");
 return INVALID_OPERATION;
}
