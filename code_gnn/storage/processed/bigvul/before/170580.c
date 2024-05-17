void LvmGlobalBundle_init(){
    ALOGV("\tLvmGlobalBundle_init start");
 for(int i=0; i<LVM_MAX_SESSIONS; i++){
 GlobalSessionMemory[i].bBundledEffectsEnabled   = LVM_FALSE;
 GlobalSessionMemory[i].bVolumeInstantiated      = LVM_FALSE;
 GlobalSessionMemory[i].bEqualizerInstantiated   = LVM_FALSE;
 GlobalSessionMemory[i].bBassInstantiated        = LVM_FALSE;
 GlobalSessionMemory[i].bVirtualizerInstantiated = LVM_FALSE;
 GlobalSessionMemory[i].pBundledContext          = LVM_NULL;

 SessionIndex[i] = LVM_UNUSED_SESSION;
 }
 return;
}
