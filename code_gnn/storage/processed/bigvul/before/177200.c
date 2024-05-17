bool ACodec::allYourBuffersAreBelongToUs() {
 return allYourBuffersAreBelongToUs(kPortIndexInput)
 && allYourBuffersAreBelongToUs(kPortIndexOutput);
}
