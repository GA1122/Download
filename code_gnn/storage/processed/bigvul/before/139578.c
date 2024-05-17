void RenderMediaClient::AddKeySystemsInfoForUMA(
    std::vector<media::KeySystemInfoForUMA>* key_systems_info_for_uma) {
  DVLOG(2) << __func__;
#if defined(WIDEVINE_CDM_AVAILABLE)
  key_systems_info_for_uma->push_back(media::KeySystemInfoForUMA(
      kWidevineKeySystem, kWidevineKeySystemNameForUMA));
#endif   
}
