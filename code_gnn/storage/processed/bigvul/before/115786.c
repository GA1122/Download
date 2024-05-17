  FakeMalwareDetails(SafeBrowsingService* sb_service,
                     TabContents* tab_contents,
                     const SafeBrowsingService::UnsafeResource& unsafe_resource)
      : MalwareDetails(sb_service, tab_contents, unsafe_resource) { }
