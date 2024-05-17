void TestingAutomationProvider::SetOmniboxText(Browser* browser,
                                               DictionaryValue* args,
                                               IPC::Message* reply_message) {
  string16 text;
  AutomationJSONReply reply(this, reply_message);
  if (!args->GetString("text", &text)) {
    reply.SendError("text missing");
    return;
  }
  browser->FocusLocationBar();
  LocationBar* loc_bar = browser->window()->GetLocationBar();
  OmniboxView* omnibox_view = loc_bar->location_entry();
  omnibox_view->model()->OnSetFocus(false);
  omnibox_view->SetUserText(text);
  reply.SendSuccess(NULL);
}
