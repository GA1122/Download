status_t IGraphicBufferProducer::QueueBufferInput::flatten(
 void*& buffer, size_t& size, int*& fds, size_t& count) const
{
 if (size < getFlattenedSize()) {
 return NO_MEMORY;
 }
 FlattenableUtils::write(buffer, size, timestamp);
 FlattenableUtils::write(buffer, size, isAutoTimestamp);
 FlattenableUtils::write(buffer, size, crop);
 FlattenableUtils::write(buffer, size, scalingMode);
 FlattenableUtils::write(buffer, size, transform);
 FlattenableUtils::write(buffer, size, async);
 return fence->flatten(buffer, size, fds, count);
}
