status_t Camera2Client::commandPingL() {
 SharedParameters::Lock l(mParameters);
 if (l.mParameters.state != Parameters::DISCONNECTED) {
 return OK;
 } else {
 return NO_INIT;
 }
}
