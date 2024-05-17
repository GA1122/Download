xmlCheckLanguageID(const xmlChar * lang)
{
    const xmlChar *cur = lang;

    if (cur == NULL)
        return (0);
    if (((cur[0] == 'i') && (cur[1] == '-')) ||
        ((cur[0] == 'I') && (cur[1] == '-'))) {
         
        cur += 2;
        while (((cur[0] >= 'A') && (cur[0] <= 'Z')) ||   
               ((cur[0] >= 'a') && (cur[0] <= 'z')))
            cur++;
    } else if (((cur[0] == 'x') && (cur[1] == '-')) ||
               ((cur[0] == 'X') && (cur[1] == '-'))) {
         
        cur += 2;
        while (((cur[0] >= 'A') && (cur[0] <= 'Z')) ||   
               ((cur[0] >= 'a') && (cur[0] <= 'z')))
            cur++;
    } else if (((cur[0] >= 'A') && (cur[0] <= 'Z')) ||
               ((cur[0] >= 'a') && (cur[0] <= 'z'))) {
         
        cur++;
        if (((cur[0] >= 'A') && (cur[0] <= 'Z')) ||
            ((cur[0] >= 'a') && (cur[0] <= 'z')))
            cur++;
        else
            return (0);
    } else
        return (0);
    while (cur[0] != 0) {        
        if (cur[0] != '-')
            return (0);
        cur++;
        if (((cur[0] >= 'A') && (cur[0] <= 'Z')) ||
            ((cur[0] >= 'a') && (cur[0] <= 'z')))
            cur++;
        else
            return (0);
        while (((cur[0] >= 'A') && (cur[0] <= 'Z')) ||   
               ((cur[0] >= 'a') && (cur[0] <= 'z')))
            cur++;
    }
    return (1);
}
