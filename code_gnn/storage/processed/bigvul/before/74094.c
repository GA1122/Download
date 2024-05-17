enum nss_status _nss_mymachines_getgrgid_r(
                gid_t gid,
                struct group *gr,
                char *buffer, size_t buflen,
                int *errnop) {

        _cleanup_bus_error_free_ sd_bus_error error = SD_BUS_ERROR_NULL;
        _cleanup_bus_message_unref_ sd_bus_message* reply = NULL;
        _cleanup_bus_flush_close_unref_ sd_bus *bus = NULL;
        const char *machine, *object;
        uint32_t mapped;
        int r;

        if (!gid_is_valid(gid)) {
                r = -EINVAL;
                goto fail;
        }

         
        if (gid < 0x10000)
                goto not_found;

        r = sd_bus_open_system(&bus);
        if (r < 0)
                goto fail;

        r = sd_bus_call_method(bus,
                               "org.freedesktop.machine1",
                               "/org/freedesktop/machine1",
                               "org.freedesktop.machine1.Manager",
                               "MapToMachineGroup",
                               &error,
                               &reply,
                               "u",
                               (uint32_t) gid);
        if (r < 0) {
                if (sd_bus_error_has_name(&error, BUS_ERROR_NO_SUCH_GROUP_MAPPING))
                        goto not_found;

                goto fail;
        }

        r = sd_bus_message_read(reply, "sou", &machine, &object, &mapped);
        if (r < 0)
                goto fail;

        if (buflen < sizeof(char*) + 1) {
                *errnop = ENOMEM;
                return NSS_STATUS_TRYAGAIN;
        }

        memzero(buffer, sizeof(char*));
        if (snprintf(buffer + sizeof(char*), buflen - sizeof(char*), "vg-%s-" GID_FMT, machine, (gid_t) mapped) >= (int) buflen) {
                *errnop = ENOMEM;
                return NSS_STATUS_TRYAGAIN;
        }

        gr->gr_name = buffer + sizeof(char*);
        gr->gr_gid = gid;
        gr->gr_passwd = (char*) "*";  
        gr->gr_mem = (char**) buffer;

        *errnop = 0;
        return NSS_STATUS_SUCCESS;

not_found:
        *errnop = 0;
        return NSS_STATUS_NOTFOUND;

fail:
        *errnop = -r;
        return NSS_STATUS_UNAVAIL;
}