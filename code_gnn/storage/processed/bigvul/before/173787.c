void SoftOpus::onPortFlushCompleted(OMX_U32 portIndex) {
 if (portIndex == 0 && mDecoder != NULL) {
        mNumFramesOutput = 0;
        opus_multistream_decoder_ctl(mDecoder, OPUS_RESET_STATE);
        mAnchorTimeUs = 0;
        mSamplesToDiscard = mSeekPreRoll;
 }
}
