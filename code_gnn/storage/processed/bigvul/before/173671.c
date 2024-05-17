status_t ATSParser::parsePID(
 ABitReader *br, unsigned PID,
 unsigned continuity_counter,
 unsigned payload_unit_start_indicator,
 SyncEvent *event) {
 ssize_t sectionIndex = mPSISections.indexOfKey(PID);

 if (sectionIndex >= 0) {
        sp<PSISection> section = mPSISections.valueAt(sectionIndex);

 if (payload_unit_start_indicator) {
 if (!section->isEmpty()) {
                ALOGW("parsePID encounters payload_unit_start_indicator when section is not empty");
                section->clear();
 }

 unsigned skip = br->getBits(8);
            section->setSkipBytes(skip + 1);  
            br->skipBits(skip * 8);
 }

 if (br->numBitsLeft() % 8 != 0) {
 return ERROR_MALFORMED;
 }
 status_t err = section->append(br->data(), br->numBitsLeft() / 8);

 if (err != OK) {
 return err;
 }

 if (!section->isComplete()) {
 return OK;
 }

 if (!section->isCRCOkay()) {
 return BAD_VALUE;
 }
 ABitReader sectionBits(section->data(), section->size());

 if (PID == 0) {
            parseProgramAssociationTable(&sectionBits);
 } else {
 bool handled = false;
 for (size_t i = 0; i < mPrograms.size(); ++i) {
 status_t err;
 if (!mPrograms.editItemAt(i)->parsePSISection(
                            PID, &sectionBits, &err)) {
 continue;
 }

 if (err != OK) {
 return err;
 }

                handled = true;
 break;
 }

 if (!handled) {
                mPSISections.removeItem(PID);
                section.clear();
 }
 }

 if (section != NULL) {
            section->clear();
 }

 return OK;
 }

 bool handled = false;
 for (size_t i = 0; i < mPrograms.size(); ++i) {
 status_t err;
 if (mPrograms.editItemAt(i)->parsePID(
                    PID, continuity_counter, payload_unit_start_indicator,
                    br, &err, event)) {
 if (err != OK) {
 return err;
 }

            handled = true;
 break;
 }
 }

 if (!handled) {
        ALOGV("PID 0x%04x not handled.", PID);
 }

 return OK;
}
