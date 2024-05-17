void Browser::ConfirmSetDefaultSearchProvider(
    TabContents* tab_contents,
    TemplateURL* template_url,
    TemplateURLService* template_url_service) {
  window()->ConfirmSetDefaultSearchProvider(tab_contents, template_url,
                                            template_url_service);
}
