debug_print_property (SmProp *prop)
{
        GString *tmp;
        int      i;

        switch (prop->type[0]) {
        case 'C':  
                g_debug ("GsmXSMPClient:   %s = %d", prop->name, *(unsigned char *)prop->vals[0].value);
                break;

        case 'A':  
                g_debug ("GsmXSMPClient:   %s = '%s'", prop->name, (char *)prop->vals[0].value);
                break;

        case 'L':  
                tmp = g_string_new (NULL);
                for (i = 0; i < prop->num_vals; i++) {
                        g_string_append_printf (tmp, "'%.*s' ", prop->vals[i].length,
                                                (char *)prop->vals[i].value);
                }
                g_debug ("GsmXSMPClient:   %s = %s", prop->name, tmp->str);
                g_string_free (tmp, TRUE);
                break;

        default:
                g_debug ("GsmXSMPClient:   %s = ??? (%s)", prop->name, prop->type);
                break;
        }
}
