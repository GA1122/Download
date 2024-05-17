FLAC__StreamEncoderWriteStatus SoftFlacEncoder::flacEncoderWriteCallback(
 const FLAC__StreamEncoder *  ,
 const FLAC__byte buffer[],
 size_t bytes,
 unsigned samples,
 unsigned current_frame,
 void *client_data) {
 return ((SoftFlacEncoder*) client_data)->onEncodedFlacAvailable(
            buffer, bytes, samples, current_frame);
}
