void ACodec::initiateCreateInputSurface() {
 (new AMessage(kWhatCreateInputSurface, this))->post();
}
