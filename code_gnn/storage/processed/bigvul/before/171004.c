status_t AudioFlinger::EffectHandle::initCheck()
{
 return mClient == 0 || mCblkMemory != 0 ? OK : NO_MEMORY;
}
