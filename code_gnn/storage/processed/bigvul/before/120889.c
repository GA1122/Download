  void LoadAuthExtension() {
    base::DictionaryValue params;

    const std::string& app_locale = g_browser_process->GetApplicationLocale();
    params.SetString("hl", app_locale);
    params.SetString("gaiaUrl", GaiaUrls::GetInstance()->gaia_url().spec());

    web_ui()->CallJavascriptFunction("inline.login.loadAuthExtension", params);
  }
