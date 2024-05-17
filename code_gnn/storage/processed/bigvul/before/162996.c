string QuicStreamSequencerBuffer::ReceivedFramesDebugString() {
  string current_frames_string;
  for (auto it : frame_arrival_time_map_) {
    QuicStreamOffset current_frame_begin_offset = it.first;
    QuicStreamOffset current_frame_end_offset =
        it.second.length + current_frame_begin_offset;
    current_frames_string.append(QuicStrCat(
        "[", current_frame_begin_offset, ", ", current_frame_end_offset,
        ") receiving time ", it.second.timestamp.ToDebuggingValue()));
  }
  return current_frames_string;
}
