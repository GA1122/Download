void OMXCodec::setState(State newState) {
    mState = newState;
    mAsyncCompletion.signal();

    mBufferFilled.signal();
}
