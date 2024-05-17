void WebContentsImpl::SetHistoryOffsetAndLength(int history_offset,
                                                int history_length) {
  SetHistoryOffsetAndLengthForView(
      GetRenderViewHost(), history_offset, history_length);
}
