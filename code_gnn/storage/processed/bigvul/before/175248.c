void RIL_setRilSocketName(const char * s) {
    strncpy(rild, s, MAX_SOCKET_NAME_LENGTH);
}
