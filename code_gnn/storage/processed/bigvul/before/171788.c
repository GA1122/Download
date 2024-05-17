static char* btif_get_default_local_name() {
 if (btif_default_local_name[0] == '\0')
 {
 int max_len = sizeof(btif_default_local_name) - 1;
 if (BTM_DEF_LOCAL_NAME[0] != '\0')
 {
            strncpy(btif_default_local_name, BTM_DEF_LOCAL_NAME, max_len);
 }
 else
 {
 char prop_model[PROPERTY_VALUE_MAX];
            property_get(PROPERTY_PRODUCT_MODEL, prop_model, "");
            strncpy(btif_default_local_name, prop_model, max_len);
 }
        btif_default_local_name[max_len] = '\0';
 }
 return btif_default_local_name;
}
