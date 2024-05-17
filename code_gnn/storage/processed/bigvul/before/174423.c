void aacDecoder_drcGetInfo (
        HANDLE_AAC_DRC  self,
        SCHAR          *pPresMode,
        SCHAR          *pProgRefLevel )
{
 if (self != NULL) {
 if (pPresMode != NULL) {
 *pPresMode = self->presMode;
 }
 if (pProgRefLevel != NULL) {
 if (self->progRefLevelPresent) {
 *pProgRefLevel = self->progRefLevel;
 } else {
 *pProgRefLevel = -1;
 }
 }
 }
}
