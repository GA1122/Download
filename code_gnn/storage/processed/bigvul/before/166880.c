void Performance::AddResourceTimingBuffer(PerformanceEntry& entry) {
  resource_timing_buffer_.push_back(&entry);

  if (IsResourceTimingBufferFull())
    DispatchEvent(Event::Create(EventTypeNames::resourcetimingbufferfull));
}
