static char* out_get_parameters(const struct audio_stream *stream, const char *keys)
{
 struct stream_out *out = (struct stream_out *)stream;
 struct str_parms *query = str_parms_create_str(keys);
 char *str;
 char value[256];
 struct str_parms *reply = str_parms_create();
 size_t i, j;
 int ret;
 bool first = true;
    ALOGV("%s: enter: keys - %s", __func__, keys);
    ret = str_parms_get_str(query, AUDIO_PARAMETER_STREAM_SUP_CHANNELS, value, sizeof(value));
 if (ret >= 0) {
        value[0] = '\0';
        i = 0;
 while (out->supported_channel_masks[i] != 0) {
 for (j = 0; j < ARRAY_SIZE(out_channels_name_to_enum_table); j++) {
 if (out_channels_name_to_enum_table[j].value == out->supported_channel_masks[i]) {
 if (!first) {
                        strcat(value, "|");
 }
                    strcat(value, out_channels_name_to_enum_table[j].name);
                    first = false;
 break;
 }
 }
            i++;
 }
        str_parms_add_str(reply, AUDIO_PARAMETER_STREAM_SUP_CHANNELS, value);
        str = str_parms_to_str(reply);
 } else {
        str = strdup(keys);
 }
    str_parms_destroy(query);
    str_parms_destroy(reply);
    ALOGV("%s: exit: returns - %s", __func__, str);
 return str;
}
