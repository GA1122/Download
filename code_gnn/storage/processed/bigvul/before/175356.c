uint32_t MyOpusExtractor::getNumSamplesInPacket(MediaBuffer *buffer) const {
 if (buffer == NULL || buffer->range_length() < 1) {
 return 0;
 }

 uint8_t *data = (uint8_t *)buffer->data() + buffer->range_offset();
 uint8_t toc = data[0];
 uint8_t config = (toc >> 3) & 0x1f;
 uint32_t frameSizesUs[] = {
 10000, 20000, 40000, 60000,  
 10000, 20000, 40000, 60000,  
 10000, 20000, 40000, 60000,  
 10000, 20000,  
 10000, 20000,  
 2500, 5000, 10000, 20000,  
 2500, 5000, 10000, 20000,  
 2500, 5000, 10000, 20000,  
 2500, 5000, 10000, 20000  
 };
 uint32_t frameSizeUs = frameSizesUs[config];

 uint32_t numFrames;
 uint8_t c = toc & 3;
 switch (c) {
 case 0:
        numFrames = 1;
 break;
 case 1:
 case 2:
        numFrames = 2;
 break;
 case 3:
 if (buffer->range_length() < 3) {
            numFrames = 0;
 } else {
            numFrames = data[2] & 0x3f;
 }
 break;
 default:
        TRESPASS();
 }

 uint32_t numSamples = frameSizeUs * numFrames * kOpusSampleRate / 1000000;
 return numSamples;
}
