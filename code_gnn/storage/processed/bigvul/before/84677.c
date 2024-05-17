kadm5_rename_principal(void *server_handle,
                       krb5_principal source, krb5_principal target)
{
    krb5_db_entry *kdb;
    osa_princ_ent_rec adb;
    krb5_error_code ret;
    kadm5_server_handle_t handle = server_handle;

    CHECK_HANDLE(server_handle);

    krb5_clear_error_message(handle->context);

    if (source == NULL || target == NULL)
        return EINVAL;

    if ((ret = kdb_get_entry(handle, target, &kdb, &adb)) == 0) {
        kdb_free_entry(handle, kdb, &adb);
        return(KADM5_DUP);
    }

    ret = k5_kadm5_hook_rename(handle->context, handle->hook_handles,
                               KADM5_HOOK_STAGE_PRECOMMIT, source, target);
    if (ret)
        return ret;

    ret = krb5_db_rename_principal(handle->context, source, target);
    if (ret)
        return ret;

     
    ret = kdb_get_entry(handle, target, &kdb, &adb);
    if (ret)
        return ret;
    kdb->mask = 0;
    ret = kdb_put_entry(handle, kdb, &adb);
    kdb_free_entry(handle, kdb, &adb);
    if (ret)
        return ret;

    (void) k5_kadm5_hook_rename(handle->context, handle->hook_handles,
                                KADM5_HOOK_STAGE_POSTCOMMIT, source, target);
    return 0;
}