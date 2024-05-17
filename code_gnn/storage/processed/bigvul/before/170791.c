status_t ESDS::getObjectTypeIndication(uint8_t *objectTypeIndication) const {
 if (mInitCheck != OK) {
 return mInitCheck;
 }

 *objectTypeIndication = mObjectTypeIndication;

 return OK;
}
