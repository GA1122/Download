void ATSParser::parseProgramAssociationTable(ABitReader *br) {
 unsigned table_id = br->getBits(8);
    ALOGV("  table_id = %u", table_id);
 if (table_id != 0x00u) {
        ALOGE("PAT data error!");
 return ;
 }
 unsigned section_syntax_indictor = br->getBits(1);
    ALOGV("  section_syntax_indictor = %u", section_syntax_indictor);

    br->skipBits(1);  
    MY_LOGV("  reserved = %u", br->getBits(2));

 unsigned section_length = br->getBits(12);
    ALOGV("  section_length = %u", section_length);

    MY_LOGV("  transport_stream_id = %u", br->getBits(16));
    MY_LOGV("  reserved = %u", br->getBits(2));
    MY_LOGV("  version_number = %u", br->getBits(5));
    MY_LOGV("  current_next_indicator = %u", br->getBits(1));
    MY_LOGV("  section_number = %u", br->getBits(8));
    MY_LOGV("  last_section_number = %u", br->getBits(8));

 size_t numProgramBytes = (section_length - 5   - 4  );

 for (size_t i = 0; i < numProgramBytes / 4; ++i) {
 unsigned program_number = br->getBits(16);
        ALOGV("    program_number = %u", program_number);

        MY_LOGV("    reserved = %u", br->getBits(3));

 if (program_number == 0) {
            MY_LOGV("    network_PID = 0x%04x", br->getBits(13));
 } else {
 unsigned programMapPID = br->getBits(13);

            ALOGV("    program_map_PID = 0x%04x", programMapPID);

 bool found = false;
 for (size_t index = 0; index < mPrograms.size(); ++index) {
 const sp<Program> &program = mPrograms.itemAt(index);

 if (program->number() == program_number) {
                    program->updateProgramMapPID(programMapPID);
                    found = true;
 break;
 }
 }

 if (!found) {
                mPrograms.push(
 new Program(this, program_number, programMapPID, mLastRecoveredPTS));
 }

 if (mPSISections.indexOfKey(programMapPID) < 0) {
                mPSISections.add(programMapPID, new PSISection);
 }
 }
 }

    MY_LOGV("  CRC = 0x%08x", br->getBits(32));
}
