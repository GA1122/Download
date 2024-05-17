void SearchProviderTest::QueryForInput(const string16& text) {
  AutocompleteInput input(UTF16ToWide(text), std::wstring(),
                          false, false, false);
  provider_->Start(input, false);

  message_loop_.RunAllPending();
}
