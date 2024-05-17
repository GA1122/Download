void Browser::WebIntentDispatch(TabContents* tab,
                                int routing_id,
                                const string16& action,
                                const string16& type,
                                const string16& data,
                                int intent_id) {
  if (!CommandLine::ForCurrentProcess()->HasSwitch(switches::kEnableWebIntents))
    return;

  TabContentsWrapper* tcw =
      TabContentsWrapper::GetCurrentWrapperForContents(tab);

  tcw->web_intent_picker_controller()->ShowDialog(window()->GetNativeHandle(),
                                                  action, type);
}
