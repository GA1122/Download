media_status_t AMediaCodec_queueSecureInputBuffer(
 AMediaCodec* codec,
 size_t idx,
 off_t offset,
 AMediaCodecCryptoInfo* crypto,
 uint64_t time,
 uint32_t flags) {

 CryptoPlugin::SubSample *subSamples = new CryptoPlugin::SubSample[crypto->numsubsamples];
 for (int i = 0; i < crypto->numsubsamples; i++) {
        subSamples[i].mNumBytesOfClearData = crypto->clearbytes[i];
        subSamples[i].mNumBytesOfEncryptedData = crypto->encryptedbytes[i];
 }

 CryptoPlugin::Pattern pattern;
    pattern.mEncryptBlocks = crypto->pattern.encryptBlocks;
    pattern.mSkipBlocks = crypto->pattern.skipBlocks;

 AString errormsg;
 status_t err  = codec->mCodec->queueSecureInputBuffer(idx,
            offset,
            subSamples,
            crypto->numsubsamples,
            crypto->key,
            crypto->iv,
 (CryptoPlugin::Mode)crypto->mode,
            pattern,
            time,
            flags,
 &errormsg);
 if (err != 0) {
        ALOGE("queSecureInputBuffer: %s", errormsg.c_str());
 }
 delete [] subSamples;
 return translate_error(err);
}
