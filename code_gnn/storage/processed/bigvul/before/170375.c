status_t MPEG4Source::parseTrackFragmentRun(off64_t offset, off64_t size) {

    ALOGV("MPEG4Extractor::parseTrackFragmentRun");
 if (size < 8) {
 return -EINVAL;
 }

 enum {
        kDataOffsetPresent                  = 0x01,
        kFirstSampleFlagsPresent            = 0x04,
        kSampleDurationPresent              = 0x100,
        kSampleSizePresent                  = 0x200,
        kSampleFlagsPresent                 = 0x400,
        kSampleCompositionTimeOffsetPresent = 0x800,
 };

 uint32_t flags;
 if (!mDataSource->getUInt32(offset, &flags)) {
 return ERROR_MALFORMED;
 }
    ALOGV("fragment run flags: %08x", flags);

 if (flags & 0xff000000) {
 return -EINVAL;
 }

 if ((flags & kFirstSampleFlagsPresent) && (flags & kSampleFlagsPresent)) {
 return -EINVAL;
 }

 uint32_t sampleCount;
 if (!mDataSource->getUInt32(offset + 4, &sampleCount)) {
 return ERROR_MALFORMED;
 }
    offset += 8;
    size -= 8;

 uint64_t dataOffset = mTrackFragmentHeaderInfo.mDataOffset;

 uint32_t firstSampleFlags = 0;

 if (flags & kDataOffsetPresent) {
 if (size < 4) {
 return -EINVAL;
 }

 int32_t dataOffsetDelta;
 if (!mDataSource->getUInt32(offset, (uint32_t*)&dataOffsetDelta)) {
 return ERROR_MALFORMED;
 }

        dataOffset = mTrackFragmentHeaderInfo.mBaseDataOffset + dataOffsetDelta;

        offset += 4;
        size -= 4;
 }

 if (flags & kFirstSampleFlagsPresent) {
 if (size < 4) {
 return -EINVAL;
 }

 if (!mDataSource->getUInt32(offset, &firstSampleFlags)) {
 return ERROR_MALFORMED;
 }
        offset += 4;
        size -= 4;
 }

 uint32_t sampleDuration = 0, sampleSize = 0, sampleFlags = 0,
             sampleCtsOffset = 0;

 size_t bytesPerSample = 0;
 if (flags & kSampleDurationPresent) {
        bytesPerSample += 4;
 } else if (mTrackFragmentHeaderInfo.mFlags
 & TrackFragmentHeaderInfo::kDefaultSampleDurationPresent) {
        sampleDuration = mTrackFragmentHeaderInfo.mDefaultSampleDuration;
 } else if (mTrex) {
        sampleDuration = mTrex->default_sample_duration;
 }

 if (flags & kSampleSizePresent) {
        bytesPerSample += 4;
 } else if (mTrackFragmentHeaderInfo.mFlags
 & TrackFragmentHeaderInfo::kDefaultSampleSizePresent) {
        sampleSize = mTrackFragmentHeaderInfo.mDefaultSampleSize;
 } else {
        sampleSize = mTrackFragmentHeaderInfo.mDefaultSampleSize;
 }

 if (flags & kSampleFlagsPresent) {
        bytesPerSample += 4;
 } else if (mTrackFragmentHeaderInfo.mFlags
 & TrackFragmentHeaderInfo::kDefaultSampleFlagsPresent) {
        sampleFlags = mTrackFragmentHeaderInfo.mDefaultSampleFlags;
 } else {
        sampleFlags = mTrackFragmentHeaderInfo.mDefaultSampleFlags;
 }

 if (flags & kSampleCompositionTimeOffsetPresent) {
        bytesPerSample += 4;
 } else {
        sampleCtsOffset = 0;
 }

 if (size < (off64_t)sampleCount * bytesPerSample) {
 return -EINVAL;
 }

 Sample tmp;
 for (uint32_t i = 0; i < sampleCount; ++i) {
 if (flags & kSampleDurationPresent) {
 if (!mDataSource->getUInt32(offset, &sampleDuration)) {
 return ERROR_MALFORMED;
 }
            offset += 4;
 }

 if (flags & kSampleSizePresent) {
 if (!mDataSource->getUInt32(offset, &sampleSize)) {
 return ERROR_MALFORMED;
 }
            offset += 4;
 }

 if (flags & kSampleFlagsPresent) {
 if (!mDataSource->getUInt32(offset, &sampleFlags)) {
 return ERROR_MALFORMED;
 }
            offset += 4;
 }

 if (flags & kSampleCompositionTimeOffsetPresent) {
 if (!mDataSource->getUInt32(offset, &sampleCtsOffset)) {
 return ERROR_MALFORMED;
 }
            offset += 4;
 }

        ALOGV("adding sample %d at offset 0x%08" PRIx64 ", size %u, duration %u, "
 " flags 0x%08x", i + 1,
                dataOffset, sampleSize, sampleDuration,
 (flags & kFirstSampleFlagsPresent) && i == 0
 ? firstSampleFlags : sampleFlags);
        tmp.offset = dataOffset;
        tmp.size = sampleSize;
        tmp.duration = sampleDuration;
        tmp.compositionOffset = sampleCtsOffset;
        mCurrentSamples.add(tmp);

        dataOffset += sampleSize;
 }

    mTrackFragmentHeaderInfo.mDataOffset = dataOffset;

 return OK;
}
