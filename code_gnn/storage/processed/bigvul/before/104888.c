PP_Bool PPB_URLLoader_Impl::GetUploadProgress(int64_t* bytes_sent,
                                              int64_t* total_bytes_to_be_sent) {
  if (!RecordUploadProgress()) {
    *bytes_sent = 0;
    *total_bytes_to_be_sent = 0;
    return PP_FALSE;
  }
  *bytes_sent = bytes_sent_;
  *total_bytes_to_be_sent = total_bytes_to_be_sent_;
  return PP_TRUE;
}
