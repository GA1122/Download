bool StreamingProcessor::isStreamActive(const Vector<int32_t> &streams,
 int32_t recordingStreamId) {
 for (size_t i = 0; i < streams.size(); i++) {
 if (streams[i] == recordingStreamId) {
 return true;
 }
 }
 return false;
}
