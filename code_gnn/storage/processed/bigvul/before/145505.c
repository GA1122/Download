bool ResourceDispatcherHostImpl::LoadInfoIsMoreInteresting(const LoadInfo& a,
                                                           const LoadInfo& b) {

  uint64_t a_uploading_size = 0;
  if (a.load_state.state == net::LOAD_STATE_SENDING_REQUEST)
    a_uploading_size = a.upload_size;

  uint64_t b_uploading_size = 0;
  if (b.load_state.state == net::LOAD_STATE_SENDING_REQUEST)
    b_uploading_size = b.upload_size;

  if (a_uploading_size != b_uploading_size)
    return a_uploading_size > b_uploading_size;

  return a.load_state.state > b.load_state.state;
}
