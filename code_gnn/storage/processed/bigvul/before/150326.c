PasswordAutofillAgent::PasswordAutofillAgent(
    content::RenderFrame* render_frame,
    blink::AssociatedInterfaceRegistry* registry)
    : content::RenderFrameObserver(render_frame),
      last_supplied_password_info_iter_(web_input_to_password_info_.end()),
      logging_state_active_(false),
      username_autofill_state_(WebAutofillState::kNotFilled),
      password_autofill_state_(WebAutofillState::kNotFilled),
      sent_request_to_store_(false),
      checked_safe_browsing_reputation_(false),
      focus_state_notifier_(this),
      password_generation_agent_(nullptr) {
  registry->AddInterface(base::Bind(&PasswordAutofillAgent::BindPendingReceiver,
                                    base::Unretained(this)));
}
