  ChromeBrowsingDataRemoverDelegateTest()
      : profile_(new BrowsingDataRemoverTestingProfile()),
        clear_domain_reliability_tester_(profile_.get()) {
    remover_ = content::BrowserContext::GetBrowsingDataRemover(profile_.get());

#if defined(OS_ANDROID)
    static_cast<ChromeBrowsingDataRemoverDelegate*>(
        profile_->GetBrowsingDataRemoverDelegate())
        ->OverrideWebappRegistryForTesting(
            base::WrapUnique<WebappRegistry>(new TestWebappRegistry()));

    SearchPermissionsService* service =
        SearchPermissionsService::Factory::GetForBrowserContext(profile_.get());
    std::unique_ptr<TestSearchEngineDelegate> delegate =
        base::MakeUnique<TestSearchEngineDelegate>();
    TestSearchEngineDelegate* delegate_ptr = delegate.get();
    service->SetSearchEngineDelegateForTest(std::move(delegate));
    delegate_ptr->UpdateDSEOrigin();
#endif
  }
