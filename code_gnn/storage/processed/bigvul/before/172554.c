StreamingProcessor::~StreamingProcessor() {
    deletePreviewStream();
    deleteRecordingStream();
}