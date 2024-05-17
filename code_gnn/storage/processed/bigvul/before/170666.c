uint32_t UuidToProcId(const effect_uuid_t * uuid)
{
 size_t i;
 for (i = 0; i < PREPROC_NUM_EFFECTS; i++) {
 if (memcmp(uuid, sUuidToPreProcTable[i], sizeof(*uuid)) == 0) {
 break;
 }
 }
 return i;
}
