check_group_membership(const char *usr, const char *grp)
{
    int index = 0;
    struct passwd *pwd = NULL;
    struct group *group = NULL;

    CRM_CHECK(usr != NULL, return FALSE);
    CRM_CHECK(grp != NULL, return FALSE);

    pwd = getpwnam(usr);
    if (pwd == NULL) {
        crm_err("No user named '%s' exists!", usr);
        return FALSE;
    }

    group = getgrgid(pwd->pw_gid);
    if (group != NULL && crm_str_eq(grp, group->gr_name, TRUE)) {
        return TRUE;
    }

    group = getgrnam(grp);
    if (group == NULL) {
        crm_err("No group named '%s' exists!", grp);
        return FALSE;
    }

    while (TRUE) {
        char *member = group->gr_mem[index++];

        if (member == NULL) {
            break;

        } else if (crm_str_eq(usr, member, TRUE)) {
            return TRUE;
        }
    };

     return FALSE;
 }
