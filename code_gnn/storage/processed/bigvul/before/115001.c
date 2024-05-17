void TestingAutomationProvider::PerformActionOnInfobar(
    Browser* browser,
    DictionaryValue* args,
    IPC::Message* reply_message) {
  AutomationJSONReply reply(this, reply_message);
  int tab_index;
  int infobar_index_int;
  std::string action;
  if (!args->GetInteger("tab_index", &tab_index) ||
      !args->GetInteger("infobar_index", &infobar_index_int) ||
      !args->GetString("action", &action)) {
    reply.SendError("Invalid or missing args");
    return;
  }

  TabContentsWrapper* tab_contents =
      browser->GetTabContentsWrapperAt(tab_index);
  if (!tab_contents) {
    reply.SendError(StringPrintf("No such tab at index %d", tab_index));
    return;
  }
  InfoBarTabHelper* infobar_helper = tab_contents->infobar_tab_helper();

  InfoBarDelegate* infobar = NULL;
  size_t infobar_index = static_cast<size_t>(infobar_index_int);
  if (infobar_index >= infobar_helper->infobar_count()) {
    reply.SendError(StringPrintf("No such infobar at index %" PRIuS,
                                 infobar_index));
    return;
  }
  infobar = infobar_helper->GetInfoBarDelegateAt(infobar_index);

  if ("dismiss" == action) {
    infobar->InfoBarDismissed();
    infobar_helper->RemoveInfoBar(infobar);
    reply.SendSuccess(NULL);
    return;
  }
  if ("accept" == action || "cancel" == action) {
    ConfirmInfoBarDelegate* confirm_infobar;
    if (!(confirm_infobar = infobar->AsConfirmInfoBarDelegate())) {
      reply.SendError("Not a confirm infobar");
      return;
    }
    if ("accept" == action) {
      if (confirm_infobar->Accept())
        infobar_helper->RemoveInfoBar(infobar);
    } else if ("cancel" == action) {
      if (confirm_infobar->Cancel())
        infobar_helper->RemoveInfoBar(infobar);
    }
    reply.SendSuccess(NULL);
    return;
  }
  reply.SendError("Invalid action");
}
