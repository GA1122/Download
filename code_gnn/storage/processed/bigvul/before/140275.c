void createDecodingBaseline(SharedBuffer* data, Vector<unsigned>* baselineHashes)
{
    OwnPtr<GIFImageDecoder> decoder = createDecoder();
    decoder->setData(data, true);
    size_t frameCount = decoder->frameCount();
    for (size_t i = 0; i < frameCount; ++i) {
        ImageFrame* frame = decoder->frameBufferAtIndex(i);
        baselineHashes->append(hashSkBitmap(frame->getSkBitmap()));
     }
 }
