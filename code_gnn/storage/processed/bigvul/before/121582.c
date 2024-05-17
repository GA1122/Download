static inline VoicedSoundMarkType composedVoicedSoundMark(UChar character)
{
    ASSERT(isKanaLetter(character));

    switch (character) {
    case 0x304C:  
    case 0x304E:  
    case 0x3050:  
    case 0x3052:  
    case 0x3054:  
    case 0x3056:  
    case 0x3058:  
    case 0x305A:  
    case 0x305C:  
    case 0x305E:  
    case 0x3060:  
    case 0x3062:  
    case 0x3065:  
    case 0x3067:  
    case 0x3069:  
    case 0x3070:  
    case 0x3073:  
    case 0x3076:  
    case 0x3079:  
    case 0x307C:  
    case 0x3094:  
    case 0x30AC:  
    case 0x30AE:  
    case 0x30B0:  
    case 0x30B2:  
    case 0x30B4:  
    case 0x30B6:  
    case 0x30B8:  
    case 0x30BA:  
    case 0x30BC:  
    case 0x30BE:  
    case 0x30C0:  
    case 0x30C2:  
    case 0x30C5:  
    case 0x30C7:  
    case 0x30C9:  
    case 0x30D0:  
    case 0x30D3:  
    case 0x30D6:  
    case 0x30D9:  
    case 0x30DC:  
    case 0x30F4:  
    case 0x30F7:  
    case 0x30F8:  
    case 0x30F9:  
    case 0x30FA:  
        return VoicedSoundMark;
    case 0x3071:  
    case 0x3074:  
    case 0x3077:  
    case 0x307A:  
    case 0x307D:  
    case 0x30D1:  
    case 0x30D4:  
    case 0x30D7:  
    case 0x30DA:  
    case 0x30DD:  
        return SemiVoicedSoundMark;
    }
    return NoVoicedSoundMark;
}
