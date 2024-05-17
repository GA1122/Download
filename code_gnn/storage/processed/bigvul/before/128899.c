    int bytesNeededToWireEncode(uint32_t value)
    {
        int bytes = 1;
        while (true) {
            value >>= varIntShift;
            if (!value)
                break;
            ++bytes;
        }

        return bytes;
    }
