bool DoUsernamesMatch(const base::string16& username1,
                      const base::string16& username2,
                      bool exact_match) {
  if (exact_match)
    return username1 == username2;
  return FieldIsSuggestionSubstringStartingOnTokenBoundary(username1, username2,
                                                           true);
}
