  void SetUserSelectedDefaultSearchProvider(const std::string& base_url,
                                            const std::string& ntp_url) {
    base::ThreadRestrictions::ScopedAllowIO allow_io;
    TemplateURLData data;
    data.SetShortName(base::UTF8ToUTF16(base_url));
    data.SetKeyword(base::UTF8ToUTF16(base_url));
    data.SetURL(base_url + "url?bar={searchTerms}");
    data.new_tab_url = ntp_url;

    TemplateURLService* template_url_service =
        TemplateURLServiceFactory::GetForProfile(browser()->profile());
    TemplateURL* template_url =
        template_url_service->Add(base::MakeUnique<TemplateURL>(data));
    template_url_service->SetUserSelectedDefaultSearchProvider(template_url);
  }
