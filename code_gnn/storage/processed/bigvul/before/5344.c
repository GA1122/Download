xps_load_sfnt_name(xps_font_t *font, char *namep, const int buflen)
{
    byte *namedata;
    int offset, length;
     
    int count, stringoffset;
    int found;
    int i, k;

    found = 0;
    strcpy(namep, "Unknown");

    offset = xps_find_sfnt_table(font, "name", &length);
    if (offset < 0 || length < 6)
    {
        gs_warn("cannot find name table");
        return;
    }

     
    if (offset + 6 > font->length)
    {
        gs_warn("name table byte offset invalid");
        return;
    }
    namedata = font->data + offset;

     
    count = u16(namedata + 2);
    stringoffset = u16(namedata + 4);

    if (stringoffset + offset > font->length
        || offset + 6 + count * 12 > font->length)
    {
        gs_warn("name table invalid");
        return;
    }

    if (length < 6 + (count * 12))
    {
        gs_warn("name table too short");
        return;
    }

    for (i = 0; i < count; i++)
    {
        byte *record = namedata + 6 + i * 12;
        int pid = u16(record + 0);
        int eid = u16(record + 2);
        int langid = u16(record + 4);
        int nameid = u16(record + 6);
        length = u16(record + 8);
        offset = u16(record + 10);

        length = length > buflen - 1 ? buflen - 1: length;

         
        if (nameid == 4 || nameid == 6)
        {
            if (pid == 1 && eid == 0 && langid == 0)  
            {
                if (found < 3)
                {
                    memcpy(namep, namedata + stringoffset + offset, length);
                    namep[length] = 0;
                    found = 3;
                }
            }

            if (pid == 3 && eid == 1 && langid == 0x409)  
            {
                if (found < 2)
                {
                    unsigned char *s = namedata + stringoffset + offset;
                    int n = length / 2;
                    for (k = 0; k < n; k ++)
                    {
                        int c = u16(s + k * 2);
                        namep[k] = isprint(c) ? c : '?';
                    }
                    namep[k] = 0;
                    found = 2;
                }
            }

            if (pid == 3 && eid == 10 && langid == 0x409)  
            {
                if (found < 1)
                {
                    unsigned char *s = namedata + stringoffset + offset;
                    int n = length / 4;
                    for (k = 0; k < n; k ++)
                    {
                        int c = u32(s + k * 4);
                        namep[k] = isprint(c) ? c : '?';
                    }
                    namep[k] = 0;
                    found = 1;
                }
            }
        }
    }
}
