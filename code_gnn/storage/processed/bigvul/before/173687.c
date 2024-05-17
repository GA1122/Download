void ATSParser::signalEOS(status_t finalResult) {
 if (finalResult == (status_t) OK) {
        ALOGE("finalResult not OK");
 return;
 }

 for (size_t i = 0; i < mPrograms.size(); ++i) {
        mPrograms.editItemAt(i)->signalEOS(finalResult);
 }
}
