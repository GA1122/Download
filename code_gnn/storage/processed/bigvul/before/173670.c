bool ATSParser::Program::parsePID(
 unsigned pid, unsigned continuity_counter,
 unsigned payload_unit_start_indicator,
 ABitReader *br, status_t *err, SyncEvent *event) {
 *err = OK;

 ssize_t index = mStreams.indexOfKey(pid);
 if (index < 0) {
 return false;
 }

 *err = mStreams.editValueAt(index)->parse(
            continuity_counter, payload_unit_start_indicator, br, event);

 return true;
}
