void ACodec::signalEndOfInputStream() {
 (new AMessage(kWhatSignalEndOfInputStream, this))->post();
}
