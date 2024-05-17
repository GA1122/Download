save_extra_data (User *user)
{
        g_autofree gchar *data = NULL;
        g_autofree gchar *filename = NULL;
        g_autoptr(GError) error = NULL;

        user_save_to_keyfile (user, user->keyfile);

        data = g_key_file_to_data (user->keyfile, NULL, &error);
        if (data == NULL) {
                g_warning ("Saving data for user %s failed: %s",
                           accounts_user_get_user_name (ACCOUNTS_USER (user)), error->message);
                return;
        }

        filename = g_build_filename (USERDIR,
                                     accounts_user_get_user_name (ACCOUNTS_USER (user)),
                                     NULL);
        g_file_set_contents (filename, data, -1, &error);
}
