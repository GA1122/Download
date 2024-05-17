void ACodec::signalFlush() {
    ALOGV("[%s] signalFlush", mComponentName.c_str());
 (new AMessage(kWhatFlush, this))->post();
}
