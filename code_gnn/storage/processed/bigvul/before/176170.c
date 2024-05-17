media_status_t AMediaCodec_getName(
 AMediaCodec *mData,
 char** out_name) {
 if (out_name == NULL) {
 return AMEDIA_ERROR_INVALID_PARAMETER;
 }

 AString compName;
 status_t err = mData->mCodec->getName(&compName);
 if (err != OK) {
 return translate_error(err);
 }
 *out_name = strdup(compName.c_str());
 return AMEDIA_OK;
}
