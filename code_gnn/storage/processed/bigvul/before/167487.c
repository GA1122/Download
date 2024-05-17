DataPipeConsumerDispatcher::~DataPipeConsumerDispatcher() {
  DCHECK(is_closed_ && !shared_ring_buffer_.IsValid() &&
         !ring_buffer_mapping_.IsValid() && !in_transit_);
}
