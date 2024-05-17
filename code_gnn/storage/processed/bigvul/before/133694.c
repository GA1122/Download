String XSSAuditor::decodedSnippetForJavaScript(const FilterTokenRequest& request)
{
    String string = request.sourceTracker.sourceForToken(request.token);
    size_t startPosition = 0;
    size_t endPosition = string.length();
    size_t foundPosition = kNotFound;

    while (startPosition < endPosition) {
        while (startPosition < endPosition && isHTMLSpace<UChar>(string[startPosition]))
            startPosition++;

        if (request.shouldAllowCDATA)
            break;

        if (startsHTMLCommentAt(string, startPosition) || startsSingleLineCommentAt(string, startPosition)) {
            while (startPosition < endPosition && !isJSNewline(string[startPosition]))
                startPosition++;
        } else if (startsMultiLineCommentAt(string, startPosition)) {
            if (startPosition + 2 < endPosition && (foundPosition = string.find("*/", startPosition + 2)) != kNotFound)
                startPosition = foundPosition + 2;
            else
                startPosition = endPosition;
        } else
            break;
    }

    String result;
    while (startPosition < endPosition && !result.length()) {
        for (foundPosition = startPosition; foundPosition < endPosition; foundPosition++) {
            if (!request.shouldAllowCDATA) {
                if (startsSingleLineCommentAt(string, foundPosition) || startsMultiLineCommentAt(string, foundPosition)) {
                    foundPosition += 2;
                    break;
                }
                if (startsHTMLCommentAt(string, foundPosition)) {
                    foundPosition += 4;
                    break;
                }
            }
            if (string[foundPosition] == ',' || (foundPosition > startPosition + kMaximumFragmentLengthTarget && isHTMLSpace<UChar>(string[foundPosition]))) {
                break;
            }
        }

        result = fullyDecodeString(string.substring(startPosition, foundPosition - startPosition), m_encoding);
        startPosition = foundPosition + 1;
    }
    return result;
}
