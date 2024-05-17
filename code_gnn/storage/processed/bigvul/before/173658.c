sp<MediaSource> ATSParser::getSource(SourceType type) {
 int which = -1;  

 for (size_t i = 0; i < mPrograms.size(); ++i) {
 const sp<Program> &program = mPrograms.editItemAt(i);

 if (which >= 0 && (int)program->number() != which) {
 continue;
 }

        sp<MediaSource> source = program->getSource(type);

 if (source != NULL) {
 return source;
 }
 }

 return NULL;
}
