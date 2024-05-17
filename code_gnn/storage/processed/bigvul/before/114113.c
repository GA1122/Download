on_download_finished(void *user_data, Evas_Object *webview, void *event_info)
{
    Ewk_Download_Job *download = (Ewk_Download_Job *)event_info;
    info("Download finished: %s\n",  ewk_download_job_destination_get(download));
}
