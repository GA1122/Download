bool GIFLZWContext::doLZW(const unsigned char* block, size_t bytesInBlock)
{
    const size_t width = m_frameContext->width();

    if (rowIter == rowBuffer.end())
        return true;

    for (const unsigned char* ch = block; bytesInBlock-- > 0; ch++) {
        datum += ((int) *ch) << bits;
        bits += 8;

        while (bits >= codesize) {
            int code = datum & codemask;
            datum >>= codesize;
            bits -= codesize;

            if (code == clearCode) {
                codesize = m_frameContext->dataSize() + 1;
                codemask = (1 << codesize) - 1;
                avail = clearCode + 2;
                oldcode = -1;
                continue;
            }

            if (code == (clearCode + 1)) {
                if (!rowsRemaining)
                    return true;
                return false;
            }

            const int tempCode = code;
            unsigned short codeLength = 0;
            if (code < avail) {
                codeLength = suffixLength[code];
                rowIter += codeLength;
            } else if (code == avail && oldcode != -1) {
                codeLength = suffixLength[oldcode] + 1;
                rowIter += codeLength;
                *--rowIter = firstchar;
                code = oldcode;
            } else {
                return false;
            }

            while (code >= clearCode) {
                *--rowIter = suffix[code];
                code = prefix[code];
            }

            *--rowIter = firstchar = suffix[code];

            if (avail < MAX_DICTIONARY_ENTRIES && oldcode != -1) {
                prefix[avail] = oldcode;
                suffix[avail] = firstchar;
                suffixLength[avail] = suffixLength[oldcode] + 1;
                ++avail;

                if (!(avail & codemask) && avail < MAX_DICTIONARY_ENTRIES) {
                    ++codesize;
                    codemask += avail;
                }
            }
            oldcode = tempCode;
            rowIter += codeLength;

            GIFRow::iterator rowBegin = rowBuffer.begin();
            for (; rowBegin + width <= rowIter; rowBegin += width) {
                if (!outputRow(rowBegin))
                    return false;
                rowsRemaining--;
                if (!rowsRemaining)
                    return true;
            }

            if (rowBegin != rowBuffer.begin()) {
                const size_t bytesToCopy = rowIter - rowBegin;
                memcpy(rowBuffer.begin(), rowBegin, bytesToCopy);
                rowIter = rowBuffer.begin() + bytesToCopy;
            }
        }
    }
    return true;
}
