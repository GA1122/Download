void MostVisitedSitesBridge::RecordPageImpression(
    JNIEnv* env,
    const JavaParamRef<jobject>& obj,
    const JavaParamRef<jintArray>& jtile_types,
    const JavaParamRef<jintArray>& jsources,
    const JavaParamRef<jobjectArray>& jtile_urls) {
  std::vector<int> int_sources;
  base::android::JavaIntArrayToIntVector(env, jsources, &int_sources);
  std::vector<int> int_tile_types;
  base::android::JavaIntArrayToIntVector(env, jtile_types, &int_tile_types);
  std::vector<std::string> string_tile_urls;
  base::android::AppendJavaStringArrayToStringVector(env, jtile_urls,
                                                     &string_tile_urls);

  DCHECK_EQ(int_sources.size(), int_tile_types.size());
  DCHECK_EQ(int_sources.size(), string_tile_urls.size());

  std::vector<TileImpression> tiles;
  for (size_t i = 0; i < int_sources.size(); i++) {
    NTPTileSource source = static_cast<NTPTileSource>(int_sources[i]);
    MostVisitedTileType tile_type =
        static_cast<MostVisitedTileType>(int_tile_types[i]);

    tiles.emplace_back(source, tile_type, GURL(string_tile_urls[i]));
  }
  ntp_tiles::metrics::RecordPageImpression(tiles,
                                           g_browser_process->rappor_service());
}
