void SupervisedUserService::AddURLAccessRequest(const GURL& url,
                                                SuccessCallback callback) {
  GURL effective_url = policy::url_util::GetEmbeddedURL(url);
  if (!effective_url.is_valid())
    effective_url = url;
  AddPermissionRequestInternal(
      base::BindRepeating(CreateURLAccessRequest,
                          policy::url_util::Normalize(effective_url)),
      std::move(callback), 0);
}
