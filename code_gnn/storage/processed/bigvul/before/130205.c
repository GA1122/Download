void base64Encode(const char* data, unsigned len, Vector<char>& out, Base64EncodePolicy policy)
{
    out.clear();
    if (!len)
        return;

    const unsigned maxInputBufferSize = UINT_MAX / 77 * 76 / 4 * 3 - 2;
    if (len > maxInputBufferSize)
        return;

    unsigned sidx = 0;
    unsigned didx = 0;

    unsigned outLength = ((len + 2) / 3) * 4;

    bool insertLFs = (policy == Base64InsertLFs && outLength > 76);
    if (insertLFs)
        outLength += ((outLength - 1) / 76);

    int count = 0;
    out.grow(outLength);

    if (len > 1) {
        while (sidx < len - 2) {
            if (insertLFs) {
                if (count && !(count % 76))
                    out[didx++] = '\n';
                count += 4;
            }
            out[didx++] = base64EncMap[(data[sidx] >> 2) & 077];
            out[didx++] = base64EncMap[((data[sidx + 1] >> 4) & 017) | ((data[sidx] << 4) & 077)];
            out[didx++] = base64EncMap[((data[sidx + 2] >> 6) & 003) | ((data[sidx + 1] << 2) & 077)];
            out[didx++] = base64EncMap[data[sidx + 2] & 077];
            sidx += 3;
        }
    }

    if (sidx < len) {
        if (insertLFs && (count > 0) && !(count % 76))
           out[didx++] = '\n';

        out[didx++] = base64EncMap[(data[sidx] >> 2) & 077];
        if (sidx < len - 1) {
            out[didx++] = base64EncMap[((data[sidx + 1] >> 4) & 017) | ((data[sidx] << 4) & 077)];
            out[didx++] = base64EncMap[(data[sidx + 1] << 2) & 077];
        } else
            out[didx++] = base64EncMap[(data[sidx] << 4) & 077];
    }

    while (didx < out.size()) {
        out[didx] = '=';
        ++didx;
    }
}
