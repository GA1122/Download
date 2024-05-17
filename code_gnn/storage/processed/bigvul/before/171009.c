bool AudioFlinger::EffectModule::isProcessEnabled() const
{
 if (mStatus != NO_ERROR) {
 return false;
 }

 switch (mState) {
 case RESTART:
 case ACTIVE:
 case STOPPING:
 case STOPPED:
 return true;
 case IDLE:
 case STARTING:
 case DESTROYED:
 default:
 return false;
 }
}
