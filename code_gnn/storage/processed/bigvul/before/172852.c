static void btif_in_split_uuids_string_to_list(char *str, bt_uuid_t *p_uuid,
 uint32_t *p_num_uuid)
{
 char buf[64];
 char *p_start = str;
 char *p_needle;
 uint32_t num = 0;
 do
 {
        p_needle = strchr(p_start, ' ');
 if (p_needle < p_start) break;
        memset(buf, 0, sizeof(buf));
        strncpy(buf, p_start, (p_needle-p_start));
        string_to_uuid(buf, p_uuid + num);
        num++;
        p_start = ++p_needle;

 } while (*p_start != 0);
 *p_num_uuid = num;
}
