void Document::write(const SegmentedString& text,
                     Document* entered_document,
                     ExceptionState& exception_state) {
  if (ImportLoader()) {
    exception_state.ThrowDOMException(
        kInvalidStateError, "Imported document doesn't support write().");
    return;
  }

  if (!IsHTMLDocument()) {
    exception_state.ThrowDOMException(kInvalidStateError,
                                      "Only HTML documents support write().");
    return;
  }

  if (throw_on_dynamic_markup_insertion_count_) {
    exception_state.ThrowDOMException(
        kInvalidStateError,
        "Custom Element constructor should not use write().");
    return;
  }

  if (entered_document &&
      !GetSecurityOrigin()->IsSameSchemeHostPortAndSuborigin(
          entered_document->GetSecurityOrigin())) {
    exception_state.ThrowSecurityError(
        "Can only call write() on same-origin documents.");
    return;
  }

  NestingLevelIncrementer nesting_level_incrementer(write_recursion_depth_);

  write_recursion_is_too_deep_ =
      (write_recursion_depth_ > 1) && write_recursion_is_too_deep_;
  write_recursion_is_too_deep_ =
      (write_recursion_depth_ > kCMaxWriteRecursionDepth) ||
      write_recursion_is_too_deep_;

  if (write_recursion_is_too_deep_)
    return;

  bool has_insertion_point = parser_ && parser_->HasInsertionPoint();

  if (!has_insertion_point && ignore_destructive_write_count_) {
    AddConsoleMessage(
        ConsoleMessage::Create(kJSMessageSource, kWarningMessageLevel,
                               ExceptionMessages::FailedToExecute(
                                   "write", "Document",
                                   "It isn't possible to write into a document "
                                   "from an asynchronously-loaded external "
                                   "script unless it is explicitly opened.")));
    return;
  }

  if (!has_insertion_point)
    open(entered_document, ASSERT_NO_EXCEPTION);

  DCHECK(parser_);
  PerformanceMonitor::ReportGenericViolation(
      this, PerformanceMonitor::kDiscouragedAPIUse,
      "Avoid using document.write().", 0, nullptr);
  probe::breakableLocation(this, "Document.write");
  parser_->insert(text);
}
