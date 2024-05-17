status_t IGraphicBufferProducer::QueueBufferInput::unflatten(
 void const*& buffer, size_t& size, int const*& fds, size_t& count)
{
 size_t minNeeded =
 sizeof(timestamp)
 + sizeof(isAutoTimestamp)
 + sizeof(dataSpace)
 + sizeof(crop)
 + sizeof(scalingMode)
 + sizeof(transform)
 + sizeof(stickyTransform)
 + sizeof(async);

 if (size < minNeeded) {
 return NO_MEMORY;
 }

 FlattenableUtils::read(buffer, size, timestamp);
 FlattenableUtils::read(buffer, size, isAutoTimestamp);
 FlattenableUtils::read(buffer, size, dataSpace);
 FlattenableUtils::read(buffer, size, crop);
 FlattenableUtils::read(buffer, size, scalingMode);
 FlattenableUtils::read(buffer, size, transform);
 FlattenableUtils::read(buffer, size, stickyTransform);
 FlattenableUtils::read(buffer, size, async);

    fence = new Fence();
 status_t result = fence->unflatten(buffer, size, fds, count);
 if (result != NO_ERROR) {
 return result;
 }
 return surfaceDamage.unflatten(buffer, size);
}
