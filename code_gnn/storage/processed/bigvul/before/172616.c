status_t OMXNodeInstance::createPersistentInputSurface(
        sp<IGraphicBufferProducer> *bufferProducer,
        sp<IGraphicBufferConsumer> *bufferConsumer) {
 String8 name("GraphicBufferSource");

    sp<IGraphicBufferProducer> producer;
    sp<IGraphicBufferConsumer> consumer;
 BufferQueue::createBufferQueue(&producer, &consumer);
    consumer->setConsumerName(name);
    consumer->setConsumerUsageBits(GRALLOC_USAGE_HW_VIDEO_ENCODER);

    sp<BufferQueue::ProxyConsumerListener> proxy =
 new BufferQueue::ProxyConsumerListener(NULL);
 status_t err = consumer->consumerConnect(proxy, false);
 if (err != NO_ERROR) {
        ALOGE("Error connecting to BufferQueue: %s (%d)",
                strerror(-err), err);
 return err;
 }

 *bufferProducer = producer;
 *bufferConsumer = consumer;

 return OK;
}
