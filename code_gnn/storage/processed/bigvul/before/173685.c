void ATSParser::Program::signalEOS(status_t finalResult) {
 for (size_t i = 0; i < mStreams.size(); ++i) {
        mStreams.editValueAt(i)->signalEOS(finalResult);
 }
}
