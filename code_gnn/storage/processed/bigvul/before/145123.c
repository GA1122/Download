bool decodeImageData(SharedBuffer* data, bool colorCorrection, size_t packetSize)
{
    OwnPtr<ImageDecoder> decoder = ImageDecoder::create(*data,
        ImageDecoder::AlphaPremultiplied, colorCorrection ?
            ImageDecoder::GammaAndColorProfileApplied : ImageDecoder::GammaAndColorProfileIgnored);

    if (!packetSize) {
        bool allDataReceived = true;
        decoder->setData(data, allDataReceived);

        int frameCount = decoder->frameCount();
        for (int i = 0; i < frameCount; ++i) {
            if (!decoder->frameBufferAtIndex(i))
                return false;
        }

        return !decoder->failed();
    }

    RefPtr<SharedBuffer> packetData = SharedBuffer::create();
    unsigned position = 0;
    while (true) {
        const char* packet;
        unsigned length = data->getSomeData(packet, position);

        length = std::min(static_cast<size_t>(length), packetSize);
        packetData->append(packet, length);
        position += length;

        bool allDataReceived = position == data->size();
        decoder->setData(packetData.get(), allDataReceived);

        int frameCount = decoder->frameCount();
        for (int i = 0; i < frameCount; ++i) {
            if (!decoder->frameBufferAtIndex(i))
                break;
        }

        if (allDataReceived || decoder->failed())
            break;
    }

    return !decoder->failed();
}
