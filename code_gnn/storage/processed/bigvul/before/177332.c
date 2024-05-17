void ACodec::signalSubmitOutputMetadataBufferIfEOS_workaround() {
 if (mPortEOS[kPortIndexInput] && !mPortEOS[kPortIndexOutput] &&
            mMetadataBuffersToSubmit > 0) {
 (new AMessage(kWhatSubmitOutputMetadataBufferIfEOS, this))->post();
 }
}
