static gboolean highlight_forbidden(void)
{
    GList *forbidden_words = load_words_from_file(FORBIDDEN_WORDS_BLACKLLIST);
    GList *allowed_words = load_words_from_file(FORBIDDEN_WORDS_WHITELIST);

    const gboolean result = highligh_words_in_tabs(forbidden_words, allowed_words,  false);

    list_free_with_free(forbidden_words);
    list_free_with_free(allowed_words);

    return result;
}