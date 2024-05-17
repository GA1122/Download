  TestAutofillExternalDelegate(content::WebContents* web_contents,
                               AutofillManager* autofill_manager)
      : AutofillExternalDelegate(web_contents, autofill_manager),
        keyboard_listener_(NULL) {
  }
