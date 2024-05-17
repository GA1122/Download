size_t IGraphicBufferConsumer::BufferItem::getFdCount() const {
 size_t c = 0;
 if (mGraphicBuffer != 0) {
        c += mGraphicBuffer->getFdCount();
 }
 if (mFence != 0) {
        c += mFence->getFdCount();
 }
 return c;
}
