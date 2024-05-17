void ResourceMessageFilter::OnPlatformCheckSpelling(const string16& word,
                                                    int tag,
                                                    bool* correct) {
  *correct = SpellCheckerPlatform::CheckSpelling(word, tag);
}
