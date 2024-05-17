static status_t getSampleRateTableIndex(int32_t sampleRate, int32_t &index) {
 static const int32_t kSampleRateTable[] = {
 96000, 88200, 64000, 48000, 44100, 32000,
 24000, 22050, 16000, 12000, 11025, 8000
 };
 const int32_t tableSize =
 sizeof(kSampleRateTable) / sizeof(kSampleRateTable[0]);

 for (int32_t i = 0; i < tableSize; ++i) {
 if (sampleRate == kSampleRateTable[i]) {
            index = i;
 return OK;
 }
 }

 return UNKNOWN_ERROR;
}
