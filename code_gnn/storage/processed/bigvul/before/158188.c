GlobalRequestID MakeGlobalRequestID() {
  return GlobalRequestID(-1, g_next_request_id--);
}
