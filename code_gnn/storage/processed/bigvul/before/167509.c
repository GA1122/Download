DataPipeProducerDispatcher::~DataPipeProducerDispatcher() {
  DCHECK(is_closed_ && !in_transit_ && !shared_ring_buffer_.IsValid() &&
         !ring_buffer_mapping_.IsValid());
}
