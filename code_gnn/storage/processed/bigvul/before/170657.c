const effect_uuid_t * ProcIdToUuid(int procId)
{
 if (procId >= PREPROC_NUM_EFFECTS) {
 return EFFECT_UUID_NULL;
 }
 return sUuidToPreProcTable[procId];
}
