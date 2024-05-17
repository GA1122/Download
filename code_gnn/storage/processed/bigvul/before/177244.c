void ACodec::initiateStart() {
 (new AMessage(kWhatStart, this))->post();
}
