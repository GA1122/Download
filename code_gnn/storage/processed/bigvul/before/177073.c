status_t SoftMPEG2::deInitDecoder() {
 size_t i;

 if (mMemRecords) {
 iv_mem_rec_t *ps_mem_rec;

        ps_mem_rec = mMemRecords;
 for (i = 0; i < mNumMemRecords; i++) {
 if (ps_mem_rec->pv_base) {
                ivd_aligned_free(ps_mem_rec->pv_base);
 }
            ps_mem_rec++;
 }
        ivd_aligned_free(mMemRecords);
        mMemRecords = NULL;
 }

 if (mFlushOutBuffer) {
        ivd_aligned_free(mFlushOutBuffer);
        mFlushOutBuffer = NULL;
 }

    mInitNeeded = true;
    mChangingResolution = false;

 return OK;
}
