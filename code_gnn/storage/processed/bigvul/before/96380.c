static int delete_path(const char *dump_dir_name)
{
     
    if (!dir_is_in_dump_location(dump_dir_name))
    {
         
        error_msg("Bad problem directory name '%s', should start with: '%s'", dump_dir_name, g_settings_dump_location);
        return 400;  
    }
    if (!dump_dir_accessible_by_uid(dump_dir_name, client_uid))
    {
        if (errno == ENOTDIR)
        {
            error_msg("Path '%s' isn't problem directory", dump_dir_name);
            return 404;  
        }
        error_msg("Problem directory '%s' can't be accessed by user with uid %ld", dump_dir_name, (long)client_uid);
        return 403;  
    }

    delete_dump_dir(dump_dir_name);

    return 0;  
}
