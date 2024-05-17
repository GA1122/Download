LoadState MockNetworkTransaction::GetLoadState() const {
  if (data_cursor_)
    return LOAD_STATE_READING_RESPONSE;
  return LOAD_STATE_IDLE;
}
