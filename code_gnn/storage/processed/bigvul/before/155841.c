void SupervisedUserService::OnBlacklistFileChecked(const base::FilePath& path,
                                                   const GURL& url,
                                                   bool file_exists) {
  DCHECK(blacklist_state_ == BlacklistLoadState::LOAD_STARTED);
  if (file_exists) {
    LoadBlacklistFromFile(path);
    return;
  }

  DCHECK(!blacklist_downloader_);

  net::NetworkTrafficAnnotationTag traffic_annotation =
      net::DefineNetworkTrafficAnnotation("supervised_users_blacklist", R"(
        semantics {
          sender: "Supervised Users"
          description:
            "Downloads a static blacklist consisting of hostname hashes of "
            "common inappropriate websites. This is only enabled for child "
            "accounts and only if the corresponding setting is enabled by the "
            "parent."
          trigger:
            "The file is downloaded on demand if the child account profile is "
            "created and the setting is enabled."
          data:
            "No additional data is sent to the server beyond the request "
            "itself."
          destination: GOOGLE_OWNED_SERVICE
        }
        policy {
          cookies_allowed: NO
          setting:
            "The feature can be remotely enabled or disabled by the parent. In "
            "addition, if sign-in is restricted to accounts from a managed "
            "domain, those accounts are not going to be child accounts."
          chrome_policy {
            RestrictSigninToPattern {
              policy_options {mode: MANDATORY}
              RestrictSigninToPattern: "*@manageddomain.com"
            }
          }
        })");

  auto factory = content::BrowserContext::GetDefaultStoragePartition(profile_)
                     ->GetURLLoaderFactoryForBrowserProcess();
  blacklist_downloader_.reset(new FileDownloader(
      url, path, false, std::move(factory),
      base::BindOnce(&SupervisedUserService::OnBlacklistDownloadDone,
                     base::Unretained(this), path),
      traffic_annotation));
}
