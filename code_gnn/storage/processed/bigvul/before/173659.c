bool ATSParser::hasSource(SourceType type) const {
 for (size_t i = 0; i < mPrograms.size(); ++i) {
 const sp<Program> &program = mPrograms.itemAt(i);
 if (program->hasSource(type)) {
 return true;
 }
 }

 return false;
}
