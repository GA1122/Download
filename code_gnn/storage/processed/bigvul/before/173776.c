static bool ParseOpusHeader(const uint8_t *data, size_t data_size,
 OpusHeader* header) {
 const size_t kOpusHeaderSize = 19;

 const size_t kOpusHeaderChannelsOffset = 9;

 const size_t kOpusHeaderSkipSamplesOffset = 10;

 const size_t kOpusHeaderGainOffset = 16;

 const size_t kOpusHeaderChannelMappingOffset = 18;

 const size_t kOpusHeaderNumStreamsOffset = kOpusHeaderSize;
 const size_t kOpusHeaderNumCoupledOffset = kOpusHeaderNumStreamsOffset + 1;
 const size_t kOpusHeaderStreamMapOffset = kOpusHeaderNumStreamsOffset + 2;

 if (data_size < kOpusHeaderSize) {
        ALOGV("Header size is too small.");
 return false;
 }
    header->channels = *(data + kOpusHeaderChannelsOffset);

 if (header->channels <= 0 || header->channels > kMaxChannels) {
        ALOGV("Invalid Header, wrong channel count: %d", header->channels);
 return false;
 }
    header->skip_samples = ReadLE16(data, data_size,
                                        kOpusHeaderSkipSamplesOffset);
    header->gain_db = static_cast<int16_t>(
 ReadLE16(data, data_size,
                                       kOpusHeaderGainOffset));
    header->channel_mapping = *(data + kOpusHeaderChannelMappingOffset);
 if (!header->channel_mapping) {
 if (header->channels > kMaxChannelsWithDefaultLayout) {
            ALOGV("Invalid Header, missing stream map.");
 return false;
 }
        header->num_streams = 1;
        header->num_coupled = header->channels > 1;
        header->stream_map[0] = 0;
        header->stream_map[1] = 1;
 return true;
 }
 if (data_size < kOpusHeaderStreamMapOffset + header->channels) {
        ALOGV("Invalid stream map; insufficient data for current channel "
 "count: %d", header->channels);
 return false;
 }
    header->num_streams = *(data + kOpusHeaderNumStreamsOffset);
    header->num_coupled = *(data + kOpusHeaderNumCoupledOffset);
 if (header->num_streams + header->num_coupled != header->channels) {
        ALOGV("Inconsistent channel mapping.");
 return false;
 }
 for (int i = 0; i < header->channels; ++i)
      header->stream_map[i] = *(data + kOpusHeaderStreamMapOffset + i);
 return true;
}
