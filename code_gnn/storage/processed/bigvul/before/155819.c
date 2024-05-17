void CreateURLAccessRequest(const GURL& url,
                            PermissionRequestCreator* creator,
                            SupervisedUserService::SuccessCallback callback) {
  creator->CreateURLAccessRequest(url, std::move(callback));
}
