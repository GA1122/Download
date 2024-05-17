static int ComputeAbsoluteCaretPosition(size_t text_start,
                                        size_t text_length,
                                        int relative_caret_position) {
  return text_start + text_length + relative_caret_position;
}
