void HostNPScriptObject::LocalizeStrings(NPObject* localize_func) {
  DCHECK(plugin_message_loop_proxy_->BelongsToCurrentThread());

  UiStrings ui_strings;
  string16 direction;
  LocalizeString(localize_func, "@@bidi_dir", &direction);
  ui_strings.direction = UTF16ToUTF8(direction) == "rtl" ?
      remoting::UiStrings::RTL : remoting::UiStrings::LTR;
  LocalizeString(localize_func,  "PRODUCT_NAME",
                 &ui_strings.product_name);
  LocalizeString(localize_func,  "DISCONNECT_OTHER_BUTTON",
                 &ui_strings.disconnect_button_text);
  LocalizeString(localize_func,
#if defined(OS_WIN)
       "DISCONNECT_BUTTON_PLUS_SHORTCUT_WINDOWS",
#elif defined(OS_MACOSX)
       "DISCONNECT_BUTTON_PLUS_SHORTCUT_MAC_OS_X",
#else
       "DISCONNECT_BUTTON_PLUS_SHORTCUT_LINUX",
#endif
      &ui_strings.disconnect_button_text_plus_shortcut);
  LocalizeString(localize_func,  "CONTINUE_PROMPT",
                 &ui_strings.continue_prompt);
  LocalizeString(localize_func,  "CONTINUE_BUTTON",
                 &ui_strings.continue_button_text);
  LocalizeString(localize_func,  "STOP_SHARING_BUTTON",
                 &ui_strings.stop_sharing_button_text);
  LocalizeString(localize_func,  "MESSAGE_SHARED",
                 &ui_strings.disconnect_message);

  base::AutoLock auto_lock(ui_strings_lock_);
  ui_strings_ = ui_strings;
}
