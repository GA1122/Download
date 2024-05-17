on_download_request(void *user_data, Evas_Object *webview, void *event_info)
{
    Ewk_Download_Job *download = (Ewk_Download_Job *)event_info;

    Eina_Strbuf *destination_path = eina_strbuf_new();

    const char *suggested_name = ewk_download_job_suggested_filename_get(download);
    if (suggested_name && *suggested_name)
        eina_strbuf_append_printf(destination_path, "/tmp/%s", suggested_name);
    else {
        char unique_path[] = "/tmp/downloaded-file.XXXXXX";
        eina_strbuf_append(destination_path, mktemp(unique_path));
    }

    ewk_download_job_destination_set(download, eina_strbuf_string_get(destination_path));
    info("Downloading: %s\n", eina_strbuf_string_get(destination_path));
    eina_strbuf_free(destination_path);
}
