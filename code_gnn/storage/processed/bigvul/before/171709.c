static bool btif_fetch_property(const char *key, bt_bdaddr_t *addr) {
 char val[PROPERTY_VALUE_MAX] = {0};

 if (property_get(key, val, NULL)) {
 if (string_to_bdaddr(val, addr)) {
            BTIF_TRACE_DEBUG("%s: Got BDA %s", __func__, val);
 return TRUE;
 }
        BTIF_TRACE_DEBUG("%s: System Property did not contain valid bdaddr", __func__);
 }
 return FALSE;
}
