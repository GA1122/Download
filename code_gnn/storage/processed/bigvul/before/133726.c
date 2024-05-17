static bool startsHTMLCommentAt(const String& string, size_t start)
{
    return (start + 3 < string.length() && string[start] == '<' && string[start+1] == '!' && string[start+2] == '-' && string[start+3] == '-');
}
