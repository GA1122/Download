void ACodec::IdleToExecutingState::stateEntered() {
    ALOGV("[%s] Now Idle->Executing", mCodec->mComponentName.c_str());
}
