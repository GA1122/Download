void MostVisitedSitesBridge::RecordOpenedMostVisitedItem(
    JNIEnv* env,
    const JavaParamRef<jobject>& obj,
    jint index,
    jint tile_type,
    jint source) {
  ntp_tiles::metrics::RecordTileClick(
      index, static_cast<NTPTileSource>(source),
      static_cast<MostVisitedTileType>(tile_type));
}
