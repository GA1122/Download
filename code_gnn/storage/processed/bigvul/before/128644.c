  TemplateURLService* CreateTemplateURLService() {
    TemplateURLData data;
    data.SetURL("https://foobar.com/url?bar={searchTerms}");
    data.contextual_search_url = "https://foobar.com/_/contextualsearch?"
        "{google:contextualSearchVersion}{google:contextualSearchContextData}";
    TemplateURLService* template_url_service = new TemplateURLService(NULL, 0);
    TemplateURL* template_url =
        template_url_service->Add(base::MakeUnique<TemplateURL>(data));
    template_url_service->SetUserSelectedDefaultSearchProvider(template_url);
    return template_url_service;
  }
