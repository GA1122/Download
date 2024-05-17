url::Origin GetOriginForURLLoaderFactory(
    const CommonNavigationParams& common_params) {
  GURL target_url = common_params.url;
  if (target_url.SchemeIs(url::kAboutScheme))
    return common_params.initiator_origin.value_or(url::Origin());

  return url::Origin::Create(target_url);
}
