String XSSAuditor::decodedSnippetForAttribute(const FilterTokenRequest& request, const HTMLToken::Attribute& attribute, AttributeKind treatment)
{
    int start = attribute.nameRange.start - request.token.startIndex();
    int end = attribute.valueRange.end - request.token.startIndex();
    String decodedSnippet = fullyDecodeString(request.sourceTracker.sourceForToken(request.token).substring(start, end - start), m_encoding);
    decodedSnippet.truncate(kMaximumFragmentLengthTarget);
    if (treatment == SrcLikeAttribute) {
        int slashCount = 0;
        bool commaSeen = false;
        for (size_t currentLength = 0; currentLength < decodedSnippet.length(); ++currentLength) {
            UChar currentChar = decodedSnippet[currentLength];
            if (currentChar == '?'
                || currentChar == '#'
                || ((currentChar == '/' || currentChar == '\\') && (commaSeen || ++slashCount > 2))
                || (currentChar == '<' && commaSeen)) {
                decodedSnippet.truncate(currentLength);
                break;
            }
            if (currentChar == ',')
                commaSeen = true;
        }
    } else if (treatment == ScriptLikeAttribute) {
        size_t position = 0;
        if ((position = decodedSnippet.find("=")) != kNotFound
            && (position = decodedSnippet.find(isNotHTMLSpace<UChar>, position + 1)) != kNotFound
            && (position = decodedSnippet.find(isTerminatingCharacter, isHTMLQuote(decodedSnippet[position]) ? position + 1 : position)) != kNotFound) {
            decodedSnippet.truncate(position);
        }
    }
    return decodedSnippet;
}
