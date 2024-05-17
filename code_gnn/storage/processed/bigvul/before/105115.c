static inline void deleteCharacterData(PassRefPtr<CharacterData> data, unsigned startOffset, unsigned endOffset, ExceptionCode& ec)
{
    if (data->length() - endOffset)
        data->deleteData(endOffset, data->length() - endOffset, ec);
    if (startOffset)
        data->deleteData(0, startOffset, ec);
}
