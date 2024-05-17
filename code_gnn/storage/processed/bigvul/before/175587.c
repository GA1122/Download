status_t NuMediaExtractor::appendVorbisNumPageSamples(TrackInfo *info, const sp<ABuffer> &buffer) {
 int32_t numPageSamples;
 if (!info->mSample->meta_data()->findInt32(
            kKeyValidSamples, &numPageSamples)) {
        numPageSamples = -1;
 }

    memcpy((uint8_t *)buffer->data() + info->mSample->range_length(),
 &numPageSamples,
 sizeof(numPageSamples));

 uint32_t type;
 const void *data;
 size_t size, size2;
 if (info->mSample->meta_data()->findData(kKeyEncryptedSizes, &type, &data, &size)) {
 if (SIZE_MAX - size < sizeof(int32_t)) {
 return -ENOMEM;
 }

 size_t newSize = size + sizeof(int32_t);
        sp<ABuffer> abuf = new ABuffer(newSize);
 uint8_t *adata = static_cast<uint8_t *>(abuf->data());
 if (adata == NULL) {
 return -ENOMEM;
 }

 int32_t zero = 0;
        memcpy(adata, data, size);
        memcpy(adata + size, &zero, sizeof(zero));
        info->mSample->meta_data()->setData(kKeyEncryptedSizes, type, adata, newSize);

 if (info->mSample->meta_data()->findData(kKeyPlainSizes, &type, &data, &size2)) {
 if (size2 != size) {
 return ERROR_MALFORMED;
 }
            memcpy(adata, data, size);
 } else {
            memset(adata, 0, size);
 }
 int32_t int32Size = sizeof(numPageSamples);
        memcpy(adata + size, &int32Size, sizeof(int32Size));
        info->mSample->meta_data()->setData(kKeyPlainSizes, type, adata, newSize);
 }

 return OK;
}
