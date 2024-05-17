status_t IGraphicBufferProducer::QueueBufferInput::unflatten(
 void const*& buffer, size_t& size, int const*& fds, size_t& count)
{
 size_t minNeeded =
 sizeof(timestamp)
 + sizeof(isAutoTimestamp)
 + sizeof(crop)
 + sizeof(scalingMode)
 + sizeof(transform)
 + sizeof(async);

 if (size < minNeeded) {
 return NO_MEMORY;
 }

 FlattenableUtils::read(buffer, size, timestamp);
 FlattenableUtils::read(buffer, size, isAutoTimestamp);
 FlattenableUtils::read(buffer, size, crop);
 FlattenableUtils::read(buffer, size, scalingMode);
 FlattenableUtils::read(buffer, size, transform);
 FlattenableUtils::read(buffer, size, async);

    fence = new Fence();
 return fence->unflatten(buffer, size, fds, count);
}
