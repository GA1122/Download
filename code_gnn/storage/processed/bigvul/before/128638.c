base::string16 ContextualSearchDelegate::SurroundingTextForIcing(
    const base::string16& surrounding_text,
    int padding_each_side,
    size_t* start,
    size_t* end) {
  base::string16 result_text = surrounding_text;
  size_t start_offset = *start;
  size_t end_offset = *end;
  size_t padding_each_side_pinned =
      padding_each_side >= 0 ? padding_each_side : 0;
  if (start_offset > padding_each_side_pinned) {
    int trim = start_offset - padding_each_side_pinned;
    result_text = result_text.substr(trim);
    start_offset -= trim;
    end_offset -= trim;
  }
  if (result_text.length() > end_offset + padding_each_side_pinned) {
    result_text = result_text.substr(0, end_offset + padding_each_side_pinned);
  }
  *start = start_offset;
  *end = end_offset;
  return result_text;
}
