size_t IGraphicBufferConsumer::BufferItem::getFlattenedSize() const {
 size_t c = 0;
 if (mGraphicBuffer != 0) {
        c += mGraphicBuffer->getFlattenedSize();
        c = FlattenableUtils::align<4>(c);
 }
 if (mFence != 0) {
        c += mFence->getFlattenedSize();
        c = FlattenableUtils::align<4>(c);
 }
 return sizeof(int32_t) + c + getPodSize();
}
