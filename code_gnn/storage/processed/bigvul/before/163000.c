void QuicStreamSequencerBuffer::UpdateFrameArrivalMap(QuicStreamOffset offset) {
  auto next_frame = frame_arrival_time_map_.upper_bound(offset);
  DCHECK(next_frame != frame_arrival_time_map_.begin());
  auto iter = frame_arrival_time_map_.begin();
  while (iter != next_frame) {
    auto erased = *iter;
    iter = frame_arrival_time_map_.erase(iter);
    QUIC_DVLOG(1) << "Removed FrameInfo with offset: " << erased.first
                  << " and length: " << erased.second.length;
    if (erased.first + erased.second.length > offset) {
      auto updated = std::make_pair(
          offset, FrameInfo(erased.first + erased.second.length - offset,
                            erased.second.timestamp));
      QUIC_DVLOG(1) << "Inserted FrameInfo with offset: " << updated.first
                    << " and length: " << updated.second.length;
      frame_arrival_time_map_.insert(updated);
    }
  }
}
