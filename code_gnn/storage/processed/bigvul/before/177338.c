void ACodec::OutputPortSettingsChangedState::stateEntered() {
    ALOGV("[%s] Now handling output port settings change",
         mCodec->mComponentName.c_str());
}
