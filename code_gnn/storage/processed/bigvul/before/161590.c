void AddSearchStrings(content::WebUIDataSource* html_source, Profile* profile) {
#if defined(OS_CHROMEOS)
  const bool is_assistant_allowed =
      arc::IsAssistantAllowedForProfile(profile) ==
      ash::mojom::AssistantAllowedState::ALLOWED;
#endif

  LocalizedString localized_strings[] = {
#if defined(OS_CHROMEOS)
    {"searchPageTitle", is_assistant_allowed ? IDS_SETTINGS_SEARCH_AND_ASSISTANT
                                             : IDS_SETTINGS_SEARCH},
#else
    {"searchPageTitle", IDS_SETTINGS_SEARCH},
#endif
    {"searchEnginesManage", IDS_SETTINGS_SEARCH_MANAGE_SEARCH_ENGINES},
#if defined(OS_CHROMEOS)
    {"searchGoogleAssistant", IDS_SETTINGS_SEARCH_GOOGLE_ASSISTANT},
    {"searchGoogleAssistantEnabled",
     IDS_SETTINGS_SEARCH_GOOGLE_ASSISTANT_ENABLED},
    {"searchGoogleAssistantDisabled",
     IDS_SETTINGS_SEARCH_GOOGLE_ASSISTANT_DISABLED},
    {"assistantTurnOn", IDS_SETTINGS_SEARCH_GOOGLE_ASSISTANT_TURN_ON},
#endif
  };
  AddLocalizedStringsBulk(html_source, localized_strings,
                          arraysize(localized_strings));
  base::string16 search_explanation_text = l10n_util::GetStringFUTF16(
      IDS_SETTINGS_SEARCH_EXPLANATION,
      base::ASCIIToUTF16(chrome::kOmniboxLearnMoreURL));
  html_source->AddString("searchExplanation", search_explanation_text);
#if defined(OS_CHROMEOS)
  html_source->AddBoolean("enableVoiceInteraction", is_assistant_allowed);
#endif
}
