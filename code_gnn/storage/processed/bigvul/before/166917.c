void Performance::setResourceTimingBufferSize(unsigned size) {
  resource_timing_buffer_size_ = size;
  if (IsResourceTimingBufferFull())
    DispatchEvent(Event::Create(EventTypeNames::resourcetimingbufferfull));
}
