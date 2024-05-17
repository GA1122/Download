bool Camera2Client::recordingEnabledL() {
    ATRACE_CALL();
 SharedParameters::Lock l(mParameters);

 return (l.mParameters.state == Parameters::RECORD
 || l.mParameters.state == Parameters::VIDEO_SNAPSHOT);
}
