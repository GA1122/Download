int64_t MyOpusExtractor::getTimeUsOfGranule(uint64_t granulePos) const {
 uint64_t pcmSamplePosition = 0;
 if (granulePos > mCodecDelay) {
        pcmSamplePosition = granulePos - mCodecDelay;
 }
 if (pcmSamplePosition > INT64_MAX / 1000000ll) {
 return INT64_MAX;
 }
 return pcmSamplePosition * 1000000ll / kOpusSampleRate;
}
