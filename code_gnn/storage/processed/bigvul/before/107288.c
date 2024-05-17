 void Browser::ConfirmSetDefaultSearchProvider(
     TabContentsWrapper* tab_contents,
     TemplateURL* template_url,
    TemplateURLModel* template_url_model) {
  window()->ConfirmSetDefaultSearchProvider(tab_contents, template_url,
                                            template_url_model);
}
