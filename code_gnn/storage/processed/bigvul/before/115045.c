void TestingAutomationProvider::SubmitAutofillForm(
    Browser* browser,
    DictionaryValue* args,
    IPC::Message* reply_message) {
  if (SendErrorIfModalDialogActive(this, reply_message))
    return;

  int tab_index;
  if (!args->GetInteger("tab_index", &tab_index)) {
    AutomationJSONReply(this, reply_message)
        .SendError("'tab_index' missing or invalid.");
    return;
  }
  TabContentsWrapper* tab_contents =
      browser->GetTabContentsWrapperAt(tab_index);
  if (!tab_contents) {
    AutomationJSONReply(this, reply_message).SendError(
        StringPrintf("No such tab at index %d", tab_index));
    return;
  }

  string16 frame_xpath, javascript;
  if (!args->GetString("frame_xpath", &frame_xpath)) {
    AutomationJSONReply(this, reply_message)
        .SendError("'frame_xpath' missing or invalid.");
    return;
  }
  if (!args->GetString("javascript", &javascript)) {
    AutomationJSONReply(this, reply_message)
        .SendError("'javascript' missing or invalid.");
    return;
  }

  PersonalDataManager* pdm = PersonalDataManagerFactory::GetForProfile(
      tab_contents->profile()->GetOriginalProfile());
  if (!pdm) {
    AutomationJSONReply(this, reply_message)
        .SendError("No PersonalDataManager.");
    return;
  }

  new AutofillFormSubmittedObserver(this, reply_message, pdm);

  std::string set_automation_id;
  base::SStringPrintf(&set_automation_id,
                      "window.domAutomationController.setAutomationId(%d);",
                      reply_message->routing_id());
  tab_contents->web_contents()->GetRenderViewHost()->
      ExecuteJavascriptInWebFrame(frame_xpath, UTF8ToUTF16(set_automation_id));
  tab_contents->web_contents()->GetRenderViewHost()->
      ExecuteJavascriptInWebFrame(frame_xpath, javascript);
}
