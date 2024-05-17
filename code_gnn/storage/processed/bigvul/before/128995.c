    static bool shouldSerializeDensely(uint32_t length, uint32_t propertyCount)
    {
        return 6 * propertyCount >= length;
    }
