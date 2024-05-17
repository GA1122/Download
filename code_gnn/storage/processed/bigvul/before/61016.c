compress_job_on_progress (AutoarCompressor *compressor,
                          guint64           completed_size,
                          guint             completed_files,
                          gpointer          user_data)
{
    CompressJob *compress_job = user_data;
    CommonJob *common = user_data;
    char *status;
    char *details;
    int files_left;
    double elapsed;
    double transfer_rate;
    int remaining_time;

    files_left = compress_job->total_files - completed_files;

    if (compress_job->total_files == 1)
    {
        status = f (_("Compressing “%B” into “%B”"),
                    G_FILE (compress_job->source_files->data),
                    compress_job->output_file);
    }
    else
    {
        status = f (ngettext ("Compressing %'d file into “%B”",
                              "Compressing %'d files into “%B”",
                              compress_job->total_files),
                    compress_job->total_files,
                    compress_job->output_file);
    }

    nautilus_progress_info_take_status (common->progress, status);

    elapsed = g_timer_elapsed (common->time, NULL);

    transfer_rate = 0;
    remaining_time = -1;

    if (elapsed > 0)
    {
        if (completed_size > 0)
        {
            transfer_rate = completed_size / elapsed;
            remaining_time = (compress_job->total_size - completed_size) / transfer_rate;
        }
        else if (completed_files > 0)
        {
            transfer_rate = completed_files / elapsed;
            remaining_time = (compress_job->total_files - completed_files) / transfer_rate;
        }
    }

    if (elapsed < SECONDS_NEEDED_FOR_RELIABLE_TRANSFER_RATE ||
        transfer_rate == 0)
    {
        if (compress_job->total_files == 1)
        {
             
            details = f (_("%S / %S"), completed_size, compress_job->total_size);
        }
        else
        {
            details = f (_("%'d / %'d"),
                         files_left > 0 ? completed_files + 1 : completed_files,
                         compress_job->total_files);
        }
    }
    else
    {
        if (compress_job->total_files == 1)
        {
            if (files_left > 0)
            {
                 
                details = f (ngettext ("%S / %S \xE2\x80\x94 %T left (%S/sec)",
                                       "%S / %S \xE2\x80\x94 %T left (%S/sec)",
                                       seconds_count_format_time_units (remaining_time)),
                             completed_size, compress_job->total_size,
                             remaining_time,
                             (goffset) transfer_rate);
            }
            else
            {
                 
                details = f (_("%S / %S"),
                             completed_size,
                             compress_job->total_size);
            }
        }
        else
        {
            if (files_left > 0)
            {
                 
                details = f (ngettext ("%'d / %'d \xE2\x80\x94 %T left (%S/sec)",
                                       "%'d / %'d \xE2\x80\x94 %T left (%S/sec)",
                                       seconds_count_format_time_units (remaining_time)),
                             completed_files + 1, compress_job->total_files,
                             remaining_time,
                             (goffset) transfer_rate);
            }
            else
            {
                 
                details = f (_("%'d / %'d"),
                             completed_files,
                             compress_job->total_files);
            }
        }
    }

    nautilus_progress_info_take_details (common->progress, details);

    if (elapsed > SECONDS_NEEDED_FOR_APROXIMATE_TRANSFER_RATE)
    {
        nautilus_progress_info_set_remaining_time (common->progress,
                                                   remaining_time);
        nautilus_progress_info_set_elapsed_time (common->progress,
                                                 elapsed);
    }

    nautilus_progress_info_set_progress (common->progress,
                                         completed_size,
                                         compress_job->total_size);
}
