enum nss_status _nss_mymachines_getpwuid_r(
                uid_t uid,
                struct passwd *pwd,
                char *buffer, size_t buflen,
                int *errnop) {

        _cleanup_bus_error_free_ sd_bus_error error = SD_BUS_ERROR_NULL;
        _cleanup_bus_message_unref_ sd_bus_message* reply = NULL;
        _cleanup_bus_flush_close_unref_ sd_bus *bus = NULL;
        const char *machine, *object;
        uint32_t mapped;
        int r;

        if (!uid_is_valid(uid)) {
                r = -EINVAL;
                goto fail;
        }

         
        if (uid < 0x10000)
                goto not_found;

        r = sd_bus_open_system(&bus);
        if (r < 0)
                goto fail;

        r = sd_bus_call_method(bus,
                               "org.freedesktop.machine1",
                               "/org/freedesktop/machine1",
                               "org.freedesktop.machine1.Manager",
                               "MapToMachineUser",
                               &error,
                               &reply,
                               "u",
                               (uint32_t) uid);
        if (r < 0) {
                if (sd_bus_error_has_name(&error, BUS_ERROR_NO_SUCH_USER_MAPPING))
                        goto not_found;

                goto fail;
        }

        r = sd_bus_message_read(reply, "sou", &machine, &object, &mapped);
        if (r < 0)
                goto fail;

        if (snprintf(buffer, buflen, "vu-%s-" UID_FMT, machine, (uid_t) mapped) >= (int) buflen) {
                *errnop = ENOMEM;
                return NSS_STATUS_TRYAGAIN;
        }

        pwd->pw_name = buffer;
        pwd->pw_uid = uid;
        pwd->pw_gid = 65534;  
        pwd->pw_gecos = buffer;
        pwd->pw_passwd = (char*) "*";  
        pwd->pw_dir = (char*) "/";
        pwd->pw_shell = (char*) "/sbin/nologin";

        *errnop = 0;
        return NSS_STATUS_SUCCESS;

not_found:
        *errnop = 0;
        return NSS_STATUS_NOTFOUND;

fail:
        *errnop = -r;
        return NSS_STATUS_UNAVAIL;
}