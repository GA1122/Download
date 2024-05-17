void ChromotingHost::SetUiStrings(const UiStrings& ui_strings) {
  DCHECK_EQ(context_->main_message_loop(), MessageLoop::current());
  DCHECK_EQ(state_, kInitial);

   ui_strings_ = ui_strings;
 }
