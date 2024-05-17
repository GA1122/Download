void ACodec::signalRequestIDRFrame() {
 (new AMessage(kWhatRequestIDRFrame, this))->post();
}
