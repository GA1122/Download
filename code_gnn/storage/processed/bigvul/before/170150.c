void LogHTMLForm(SavePasswordProgressLogger* logger,
                 SavePasswordProgressLogger::StringID message_id,
                 const blink::WebFormElement& form) {
  logger->LogHTMLForm(message_id,
                      form.name().utf8(),
                      GURL(form.action().utf8()));
}
