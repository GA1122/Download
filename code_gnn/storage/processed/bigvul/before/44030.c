crm_xml_escape(const char *text)
{
    int index;
    int changes = 0;
    int length = 1 + strlen(text);
    char *copy = strdup(text);

     

    for (index = 0; index < length; index++) {
        switch (copy[index]) {
            case 0:
                break;
            case '<':
                copy = crm_xml_escape_shuffle(copy, index, &length, "&lt;");
                changes++;
                break;
            case '>':
                copy = crm_xml_escape_shuffle(copy, index, &length, "&gt;");
                changes++;
                break;
            case '"':
                copy = crm_xml_escape_shuffle(copy, index, &length, "&quot;");
                changes++;
                break;
            case '\'':
                copy = crm_xml_escape_shuffle(copy, index, &length, "&apos;");
                changes++;
                break;
            case '&':
                copy = crm_xml_escape_shuffle(copy, index, &length, "&amp;");
                changes++;
                break;
            case '\t':
                 
                copy = crm_xml_escape_shuffle(copy, index, &length, "    ");
                changes++;
                break;
            case '\n':
                 
                copy = crm_xml_escape_shuffle(copy, index, &length, "\\n");
                changes++;
                break;
            case '\r':
                copy = crm_xml_escape_shuffle(copy, index, &length, "\\r");
                changes++;
                break;
                 
            default:
                 
                if(copy[index] < ' ' || copy[index] > '~') {
                    char *replace = crm_strdup_printf("\\%.3o", copy[index]);

                     
                    copy = crm_xml_escape_shuffle(copy, index, &length, replace);
                    free(replace);
                    changes++;
                }
        }
    }

    if (changes) {
        crm_trace("Dumped '%s'", copy);
    }
    return copy;
}
