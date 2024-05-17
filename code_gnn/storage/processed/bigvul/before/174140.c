status_t OMX::createPersistentInputSurface(
        sp<IGraphicBufferProducer> *bufferProducer,
        sp<IGraphicBufferConsumer> *bufferConsumer) {
 return OMXNodeInstance::createPersistentInputSurface(
            bufferProducer, bufferConsumer);
}
