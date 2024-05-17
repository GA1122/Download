void PasswordAutofillAgent::SendPasswordForms(bool only_visible) {
  std::unique_ptr<RendererSavePasswordProgressLogger> logger;
  if (logging_state_active_) {
    logger.reset(new RendererSavePasswordProgressLogger(
        GetPasswordManagerDriver().get()));
    logger->LogMessage(Logger::STRING_SEND_PASSWORD_FORMS_METHOD);
    logger->LogBoolean(Logger::STRING_ONLY_VISIBLE, only_visible);
  }

  WebLocalFrame* frame = render_frame()->GetWebFrame();

  blink::WebSecurityOrigin origin = frame->GetDocument().GetSecurityOrigin();
  if (logger) {
    logger->LogURL(Logger::STRING_SECURITY_ORIGIN,
                   GURL(origin.ToString().Utf8()));
  }
  if (!FrameCanAccessPasswordManager()) {
    if (logger)
      logger->LogMessage(Logger::STRING_SECURITY_ORIGIN_FAILURE);
    return;
  }

  if (form_util::IsWebpageEmpty(frame)) {
    if (logger) {
      logger->LogMessage(Logger::STRING_WEBPAGE_EMPTY);
    }
    return;
  }

  WebVector<WebFormElement> forms;
  frame->GetDocument().Forms(forms);

  if (IsShowAutofillSignaturesEnabled())
    AnnotateFormsAndFieldsWithSignatures(frame, &forms);
  if (logger)
    logger->LogNumber(Logger::STRING_NUMBER_OF_ALL_FORMS, forms.size());

  std::vector<PasswordForm> password_forms;
  for (const WebFormElement& form : forms) {
    if (only_visible) {
      bool is_form_visible = form_util::AreFormContentsVisible(form);
      if (logger) {
        LogHTMLForm(logger.get(), Logger::STRING_FORM_FOUND_ON_PAGE, form);
        logger->LogBoolean(Logger::STRING_FORM_IS_VISIBLE, is_form_visible);
      }

      if (!is_form_visible)
        continue;
    }

    std::unique_ptr<PasswordForm> password_form(
        GetPasswordFormFromWebForm(form));
    if (!password_form)
      continue;

    if (logger)
      logger->LogPasswordForm(Logger::STRING_FORM_IS_PASSWORD, *password_form);

    FormStructureInfo form_structure_info =
        ExtractFormStructureInfo(password_form->form_data);
    if (only_visible || WasFormStructureChanged(form_structure_info)) {
      forms_structure_cache_[form_structure_info.unique_renderer_id] =
          std::move(form_structure_info);

      password_forms.push_back(std::move(*password_form));
      continue;
    }

    WebVector<WebFormControlElement> control_elements_vector;
    form.GetFormControlElements(control_elements_vector);

    std::vector<WebFormControlElement> control_elements =
        control_elements_vector.ReleaseVector();
    TryFixAutofilledForm(&control_elements);
  }

  bool add_unowned_inputs = true;
  if (only_visible) {
    std::vector<WebFormControlElement> control_elements =
        form_util::GetUnownedAutofillableFormFieldElements(
            frame->GetDocument().All(), nullptr);
    add_unowned_inputs =
        form_util::IsSomeControlElementVisible(control_elements);
    if (logger) {
      logger->LogBoolean(Logger::STRING_UNOWNED_INPUTS_VISIBLE,
                         add_unowned_inputs);
    }
  }

  if (add_unowned_inputs) {
    std::unique_ptr<PasswordForm> password_form(
        GetPasswordFormFromUnownedInputElements());
    if (password_form) {
      if (logger) {
        logger->LogPasswordForm(Logger::STRING_FORM_IS_PASSWORD,
                                *password_form);
      }

      password_forms.push_back(std::move(*password_form));
    }
  }

  if (only_visible) {
    WebFrame* main_frame = render_frame()->GetWebFrame()->Top();
    bool did_stop_loading = !main_frame || !main_frame->IsLoading();
    GetPasswordManagerDriver()->PasswordFormsRendered(password_forms,
                                                      did_stop_loading);
  } else {
    if (!sent_request_to_store_ && password_forms.empty() &&
        HasPasswordField(*frame)) {
      password_forms.push_back(PasswordForm());
      password_forms.back().scheme = PasswordForm::Scheme::kHtml;
      password_forms.back().origin =
          form_util::GetCanonicalOriginForDocument(frame->GetDocument());
      password_forms.back().signon_realm =
          GetSignOnRealm(password_forms.back().origin);
      password_forms.back().form_data.url = password_forms.back().origin;
    }
    if (!password_forms.empty()) {
      sent_request_to_store_ = true;
      GetPasswordManagerDriver()->PasswordFormsParsed(password_forms);
    }
  }

#if !defined(OS_ANDROID) && !defined(OS_IOS)
  if (!password_forms.empty() &&
      (frame->GetDocument().Url().ProtocolIs(url::kHttpScheme) ||
       frame->GetDocument().Url().ProtocolIs(url::kHttpsScheme)))
    page_passwords_analyser_.AnalyseDocumentDOM(frame);
#endif
}
