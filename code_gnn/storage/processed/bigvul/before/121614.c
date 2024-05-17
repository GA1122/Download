static inline bool isSmallKanaLetter(UChar character)
{
    ASSERT(isKanaLetter(character));

    switch (character) {
    case 0x3041:  
    case 0x3043:  
    case 0x3045:  
    case 0x3047:  
    case 0x3049:  
    case 0x3063:  
    case 0x3083:  
    case 0x3085:  
    case 0x3087:  
    case 0x308E:  
    case 0x3095:  
    case 0x3096:  
    case 0x30A1:  
    case 0x30A3:  
    case 0x30A5:  
    case 0x30A7:  
    case 0x30A9:  
    case 0x30C3:  
    case 0x30E3:  
    case 0x30E5:  
    case 0x30E7:  
    case 0x30EE:  
    case 0x30F5:  
    case 0x30F6:  
    case 0x31F0:  
    case 0x31F1:  
    case 0x31F2:  
    case 0x31F3:  
    case 0x31F4:  
    case 0x31F5:  
    case 0x31F6:  
    case 0x31F7:  
    case 0x31F8:  
    case 0x31F9:  
    case 0x31FA:  
    case 0x31FB:  
    case 0x31FC:  
    case 0x31FD:  
    case 0x31FE:  
    case 0x31FF:  
    case 0xFF67:  
    case 0xFF68:  
    case 0xFF69:  
    case 0xFF6A:  
    case 0xFF6B:  
    case 0xFF6C:  
    case 0xFF6D:  
    case 0xFF6E:  
    case 0xFF6F:  
        return true;
    }
    return false;
}
