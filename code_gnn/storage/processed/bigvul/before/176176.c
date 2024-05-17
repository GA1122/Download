void AMediaCodec_releaseName(
 AMediaCodec *  ,
 char* name) {
 if (name != NULL) {
        free(name);
 }
}
