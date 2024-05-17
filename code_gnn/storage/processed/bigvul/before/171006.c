bool AudioFlinger::EffectModule::isEnabled() const
{
 switch (mState) {
 case RESTART:
 case STARTING:
 case ACTIVE:
 return true;
 case IDLE:
 case STOPPING:
 case STOPPED:
 case DESTROYED:
 default:
 return false;
 }
}
