size_t IGraphicBufferProducer::QueueBufferInput::getFlattenedSize() const {
 return sizeof(timestamp)
 + sizeof(isAutoTimestamp)
 + sizeof(dataSpace)
 + sizeof(crop)
 + sizeof(scalingMode)
 + sizeof(transform)
 + sizeof(stickyTransform)
 + sizeof(async)
 + fence->getFlattenedSize()
 + surfaceDamage.getFlattenedSize();
}
