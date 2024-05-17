static void rehighlight_forbidden_words(int page, GtkTextView *tev)
{
    GList *forbidden_words = load_words_from_file(FORBIDDEN_WORDS_BLACKLLIST);
    GList *allowed_words = load_words_from_file(FORBIDDEN_WORDS_WHITELIST);
    highligh_words_in_textview(page, tev, forbidden_words, allowed_words,  false);
    list_free_with_free(forbidden_words);
    list_free_with_free(allowed_words);
}