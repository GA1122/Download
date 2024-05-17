static HTTPHeaderMap parseRFC822HeaderFields(const char* bytes, unsigned length)
{
    String lastHeaderKey;
    HTTPHeaderMap headerFields;

    while (const char* endOfLine = findEndOfLine(bytes, length)) {
        const char* line = bytes;
        int lineLength = endOfLine - bytes;

        bytes = endOfLine + 1;
        if ((*endOfLine == '\r') && (*bytes == '\n'))
            bytes++;  

        length -= (bytes - line);
        if (!lineLength) {
            break;
        }

        if (*line == ' ' || *line == '\t') {
            if (lastHeaderKey.isNull()) {
                continue;
            } 
            
            String currentValue = headerFields.get(lastHeaderKey);
            String newValue(line, lineLength);
            
            headerFields.set(lastHeaderKey, currentValue + newValue);
        } else {
            const char* colon = line;
            while (*colon != ':' && colon != endOfLine)
                colon++;

            if (colon == endOfLine) {
                continue;
            }

            lastHeaderKey = capitalizeRFC822HeaderFieldName(String(line, colon - line));
            String value;
            
            for (colon++; colon != endOfLine; colon++) {
                if (*colon != ' ' && *colon != '\t')
                    break;
            }
            if (colon == endOfLine)
                value = "";
            else
                value = String(colon, endOfLine - colon);
            
            String oldValue = headerFields.get(lastHeaderKey);
            if (!oldValue.isNull()) {
                String tmp = oldValue;
                tmp += ", ";
                tmp += value;
                value = tmp;
            }
            
            headerFields.set(lastHeaderKey, value);
        }
    }

    return headerFields;
}
