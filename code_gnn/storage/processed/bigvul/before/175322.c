status_t MPEG4Extractor::parseAC3SampleEntry(off64_t offset) {
    offset += 16;
 uint16_t channelCount;
 if (!mDataSource->getUInt16(offset, &channelCount)) {
 return ERROR_MALFORMED;
 }
    offset += 8;
 uint16_t sampleRate;
 if (!mDataSource->getUInt16(offset, &sampleRate)) {
        ALOGE("MPEG4Extractor: error while reading ac-3 block: cannot read sample rate");
 return ERROR_MALFORMED;
 }

    offset += 4;
 return parseAC3SpecificBox(offset, sampleRate);
}
