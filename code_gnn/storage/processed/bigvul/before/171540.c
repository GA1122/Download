void SimpleSoftOMXComponent::prepareForDestruction() {

    mLooper->unregisterHandler(mHandler->id());
    mLooper->stop();
}
