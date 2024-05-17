deep_count_one (DeepCountState *state,
                GFileInfo      *info)
{
    NautilusFile *file;
    GFile *subdir;
    gboolean is_seen_inode;
    const char *fs_id;

    if (should_skip_file (NULL, info))
    {
        return;
    }

    is_seen_inode = seen_inode (state, info);
    if (!is_seen_inode)
    {
        mark_inode_as_seen (state, info);
    }

    file = state->directory->details->deep_count_file;

    if (g_file_info_get_file_type (info) == G_FILE_TYPE_DIRECTORY)
    {
         
        file->details->deep_directory_count += 1;

         
        fs_id = g_file_info_get_attribute_string (info, G_FILE_ATTRIBUTE_ID_FILESYSTEM);
        if (g_strcmp0 (fs_id, state->fs_id) == 0)
        {
             
            subdir = g_file_get_child (state->deep_count_location, g_file_info_get_name (info));
            state->deep_count_subdirectories = g_list_prepend
                                                   (state->deep_count_subdirectories, subdir);
        }
    }
    else
    {
         
        file->details->deep_file_count += 1;
    }

     
    if (!is_seen_inode && g_file_info_has_attribute (info, G_FILE_ATTRIBUTE_STANDARD_SIZE))
    {
        file->details->deep_size += g_file_info_get_size (info);
    }
}
