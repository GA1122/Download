static inline bool isCombiningVoicedSoundMark(UChar character)
{
    switch (character) {
    case 0x3099:  
    case 0x309A:  
        return true;
    }
    return false;
}
