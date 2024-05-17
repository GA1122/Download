void AudioSource::trackMaxAmplitude(int16_t *data, int nSamples) {
 for (int i = nSamples; i > 0; --i) {
 int16_t value = *data++;
 if (value < 0) {
            value = -value;
 }
 if (mMaxAmplitude < value) {
            mMaxAmplitude = value;
 }
 }
}
