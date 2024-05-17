void ACodec::signalResume() {
 (new AMessage(kWhatResume, this))->post();
}
