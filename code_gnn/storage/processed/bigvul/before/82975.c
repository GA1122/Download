static const char *find_word_break(const char *string)
{
     
    while (string && *string && !isspace(*string))
        string++;

    return string;
}
