bool ATSParser::Program::parsePSISection(
 unsigned pid, ABitReader *br, status_t *err) {
 *err = OK;

 if (pid != mProgramMapPID) {
 return false;
 }

 *err = parseProgramMap(br);

 return true;
}
