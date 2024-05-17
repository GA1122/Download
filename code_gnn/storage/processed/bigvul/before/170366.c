status_t MPEG4Source::parseChunk(off64_t *offset) {
 uint32_t hdr[2];
 if (mDataSource->readAt(*offset, hdr, 8) < 8) {
 return ERROR_IO;
 }
 uint64_t chunk_size = ntohl(hdr[0]);
 uint32_t chunk_type = ntohl(hdr[1]);
 off64_t data_offset = *offset + 8;

 if (chunk_size == 1) {
 if (mDataSource->readAt(*offset + 8, &chunk_size, 8) < 8) {
 return ERROR_IO;
 }
        chunk_size = ntoh64(chunk_size);
        data_offset += 8;

 if (chunk_size < 16) {
 return ERROR_MALFORMED;
 }
 } else if (chunk_size < 8) {
 return ERROR_MALFORMED;
 }

 char chunk[5];
 MakeFourCCString(chunk_type, chunk);
    ALOGV("MPEG4Source chunk %s @ %llx", chunk, *offset);

 off64_t chunk_data_size = *offset + chunk_size - data_offset;

 switch(chunk_type) {

 case FOURCC('t', 'r', 'a', 'f'):
 case FOURCC('m', 'o', 'o', 'f'): {
 off64_t stop_offset = *offset + chunk_size;
 *offset = data_offset;
 while (*offset < stop_offset) {
 status_t err = parseChunk(offset);
 if (err != OK) {
 return err;
 }
 }
 if (chunk_type == FOURCC('m', 'o', 'o', 'f')) {

 while (true) {
 if (mDataSource->readAt(*offset, hdr, 8) < 8) {
 return ERROR_END_OF_STREAM;
 }
                    chunk_size = ntohl(hdr[0]);
                    chunk_type = ntohl(hdr[1]);
 if (chunk_type == FOURCC('m', 'o', 'o', 'f')) {
                        mNextMoofOffset = *offset;
 break;
 }
 *offset += chunk_size;
 }
 }
 break;
 }

 case FOURCC('t', 'f', 'h', 'd'): {
 status_t err;
 if ((err = parseTrackFragmentHeader(data_offset, chunk_data_size)) != OK) {
 return err;
 }
 *offset += chunk_size;
 break;
 }

 case FOURCC('t', 'r', 'u', 'n'): {
 status_t err;
 if (mLastParsedTrackId == mTrackId) {
 if ((err = parseTrackFragmentRun(data_offset, chunk_data_size)) != OK) {
 return err;
 }
 }

 *offset += chunk_size;
 break;
 }

 case FOURCC('s', 'a', 'i', 'z'): {
 status_t err;
 if ((err = parseSampleAuxiliaryInformationSizes(data_offset, chunk_data_size)) != OK) {
 return err;
 }
 *offset += chunk_size;
 break;
 }
 case FOURCC('s', 'a', 'i', 'o'): {
 status_t err;
 if ((err = parseSampleAuxiliaryInformationOffsets(data_offset, chunk_data_size)) != OK) {
 return err;
 }
 *offset += chunk_size;
 break;
 }

 case FOURCC('m', 'd', 'a', 't'): {
            ALOGV("MPEG4Source::parseChunk mdat");
 *offset += chunk_size;
 break;
 }

 default: {
 *offset += chunk_size;
 break;
 }
 }
 return OK;
}
