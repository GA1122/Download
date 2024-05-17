string QuicStreamSequencerBuffer::GapsDebugString() {
  string current_gaps_string;
  for (const Gap& gap : gaps_) {
    QuicStreamOffset current_gap_begin = gap.begin_offset;
    QuicStreamOffset current_gap_end = gap.end_offset;
    current_gaps_string.append(
        QuicStrCat("[", current_gap_begin, ", ", current_gap_end, ") "));
  }
  return current_gaps_string;
}
