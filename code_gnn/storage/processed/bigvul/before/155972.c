void DiceTurnSyncOnHelper::LoadPolicyWithCachedCredentials() {
  DCHECK(!dm_token_.empty());
  DCHECK(!client_id_.empty());
  policy::UserPolicySigninService* policy_service =
      policy::UserPolicySigninServiceFactory::GetForProfile(profile_);
  policy_service->FetchPolicyForSignedInUser(
      AccountIdFromAccountInfo(account_info_), dm_token_, client_id_,
      content::BrowserContext::GetDefaultStoragePartition(profile_)
          ->GetURLLoaderFactoryForBrowserProcess(),
      base::Bind(&DiceTurnSyncOnHelper::OnPolicyFetchComplete,
                 weak_pointer_factory_.GetWeakPtr()));
}
