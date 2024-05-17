static sp<MediaSource> InstantiateSoftwareEncoder(
 const char *name, const sp<MediaSource> &source,
 const sp<MetaData> &meta) {
 struct FactoryInfo {
 const char *name;
        sp<MediaSource> (*CreateFunc)(const sp<MediaSource> &, const sp<MetaData> &);
 };

 static const FactoryInfo kFactoryInfo[] = {
        FACTORY_REF(AACEncoder)
 };
 for (size_t i = 0;
         i < sizeof(kFactoryInfo) / sizeof(kFactoryInfo[0]); ++i) {
 if (!strcmp(name, kFactoryInfo[i].name)) {
 return (*kFactoryInfo[i].CreateFunc)(source, meta);
 }
 }

 return NULL;
}
