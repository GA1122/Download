ResourceDispatcherHostImpl::PickMoreInterestingLoadInfos(
    std::unique_ptr<LoadInfoList> infos) {
  auto info_map = std::make_unique<LoadInfoMap>();
  for (const auto& load_info : *infos) {
    WebContents* web_contents = load_info.web_contents_getter.Run();
    if (!web_contents)
      continue;

    auto existing = info_map->find(web_contents);
    if (existing == info_map->end() ||
        LoadInfoIsMoreInteresting(load_info, existing->second)) {
      (*info_map)[web_contents] = load_info;
    }
  }
  return info_map;
}
