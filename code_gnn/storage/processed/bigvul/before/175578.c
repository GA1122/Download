sp<MetaData> AMRExtractor::getMetaData() {
    sp<MetaData> meta = new MetaData;

 if (mInitCheck != OK) {
 return meta;
 }

    meta->setCString(kKeyMIMEType, mIsWide ? "audio/amr-wb" : "audio/amr");

 return meta;
}
