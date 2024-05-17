status_t ATSParser::parseAdaptationField(ABitReader *br, unsigned PID) {
 unsigned adaptation_field_length = br->getBits(8);

 if (adaptation_field_length > 0) {
 if (adaptation_field_length * 8 > br->numBitsLeft()) {
            ALOGV("Adaptation field should be included in a single TS packet.");
 return ERROR_MALFORMED;
 }

 unsigned discontinuity_indicator = br->getBits(1);

 if (discontinuity_indicator) {
            ALOGV("PID 0x%04x: discontinuity_indicator = 1 (!!!)", PID);
 }

        br->skipBits(2);
 unsigned PCR_flag = br->getBits(1);

 size_t numBitsRead = 4;

 if (PCR_flag) {
 if (adaptation_field_length * 8 < 52) {
 return ERROR_MALFORMED;
 }
            br->skipBits(4);
 uint64_t PCR_base = br->getBits(32);
            PCR_base = (PCR_base << 1) | br->getBits(1);

            br->skipBits(6);
 unsigned PCR_ext = br->getBits(9);

 size_t byteOffsetFromStartOfTSPacket =
 (188 - br->numBitsLeft() / 8);

 uint64_t PCR = PCR_base * 300 + PCR_ext;

            ALOGV("PID 0x%04x: PCR = 0x%016" PRIx64 " (%.2f)",
                  PID, PCR, PCR / 27E6);

 size_t byteOffsetFromStart =
                mNumTSPacketsParsed * 188 + byteOffsetFromStartOfTSPacket;

 for (size_t i = 0; i < mPrograms.size(); ++i) {
                updatePCR(PID, PCR, byteOffsetFromStart);
 }

            numBitsRead += 52;
 }

        br->skipBits(adaptation_field_length * 8 - numBitsRead);
 }
 return OK;
}
