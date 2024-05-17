parse_key_string(gchar* input_string, guint8 key_type)
{
    gchar *key, *tmp_str;
    gchar *ssid;

    GString    *key_string = NULL;
    GByteArray *ssid_ba = NULL, *key_ba;
    gboolean    res;

    gchar **tokens;
    guint n = 0;
    decryption_key_t *dk;

    if(input_string == NULL)
        return NULL;

     

    switch(key_type)
    {
    case AIRPDCAP_KEY_TYPE_WEP:
    case AIRPDCAP_KEY_TYPE_WEP_40:
    case AIRPDCAP_KEY_TYPE_WEP_104:

       key_ba = g_byte_array_new();
       res = hex_str_to_bytes(input_string, key_ba, FALSE);

       if (res && key_ba->len > 0) {
            
            
           dk = (decryption_key_t *)g_malloc(sizeof(decryption_key_t));

           dk->type = AIRPDCAP_KEY_TYPE_WEP;
            
           tmp_str = bytes_to_str(NULL, key_ba->data, key_ba->len);
           dk->key  = g_string_new(tmp_str);
           g_string_ascii_down(dk->key);
           dk->bits = key_ba->len * 8;
           dk->ssid = NULL;

           wmem_free(NULL, tmp_str);
           g_byte_array_free(key_ba, TRUE);
           return dk;
       }

        
       g_byte_array_free(key_ba, TRUE);
       return NULL;

    case AIRPDCAP_KEY_TYPE_WPA_PWD:

        tokens = g_strsplit(input_string,":",0);

         
        while(tokens[n] != NULL)
            n++;

        if(n < 1)
        {
             
            g_strfreev(tokens);
            return NULL;
        }

         
        key = g_strdup(tokens[0]);

        ssid = NULL;
         
        if(n >= 2)
        {
           ssid = g_strdup(tokens[1]);
        }

         
        key_string = g_string_new(key);
        ssid_ba = NULL;

         
        if( ((key_string->len) > WPA_KEY_MAX_CHAR_SIZE) || ((key_string->len) < WPA_KEY_MIN_CHAR_SIZE))
        {
            g_string_free(key_string, TRUE);

            g_free(key);
            g_free(ssid);

             
            g_strfreev(tokens);
            return NULL;
        }

        if(ssid != NULL)  
        {
            ssid_ba = g_byte_array_new();
            if (! uri_str_to_bytes(ssid, ssid_ba)) {
                g_string_free(key_string, TRUE);
                g_byte_array_free(ssid_ba, TRUE);
                g_free(key);
                g_free(ssid);
                 
                g_strfreev(tokens);
                return NULL;
            }

            if(ssid_ba->len > WPA_SSID_MAX_CHAR_SIZE)
            {
                g_string_free(key_string, TRUE);
                g_byte_array_free(ssid_ba, TRUE);

                g_free(key);
                g_free(ssid);

                 
                g_strfreev(tokens);
                return NULL;
            }
        }

         
        dk = (decryption_key_t*)g_malloc(sizeof(decryption_key_t));

        dk->type = AIRPDCAP_KEY_TYPE_WPA_PWD;
        dk->key  = g_string_new(key);
        dk->bits = 256;  
        dk->ssid = byte_array_dup(ssid_ba);  

        g_string_free(key_string, TRUE);
        if (ssid_ba != NULL)
            g_byte_array_free(ssid_ba, TRUE);

        g_free(key);
        if(ssid != NULL)
            g_free(ssid);

         
        g_strfreev(tokens);
        return dk;

    case AIRPDCAP_KEY_TYPE_WPA_PSK:

        key_ba = g_byte_array_new();
        res = hex_str_to_bytes(input_string, key_ba, FALSE);

         
        if(!res || ((key_ba->len) != WPA_PSK_KEY_SIZE))
        {
            g_byte_array_free(key_ba, TRUE);

             
            return NULL;
        }

         
        dk = (decryption_key_t*)g_malloc(sizeof(decryption_key_t));

        dk->type = AIRPDCAP_KEY_TYPE_WPA_PSK;
        dk->key  = g_string_new(input_string);
        dk->bits = (guint) dk->key->len * 4;
        dk->ssid = NULL;

        g_byte_array_free(key_ba, TRUE);
        return dk;
    }

     
    return NULL;
}
