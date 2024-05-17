static const char* inspectorFilesBasePath()
{
    const gchar* environmentPath = g_getenv("WEBKIT_INSPECTOR_PATH");
    if (environmentPath && g_file_test(environmentPath, G_FILE_TEST_IS_DIR))
        return environmentPath;

    static const char* inspectorFilesPath = DATA_DIR""G_DIR_SEPARATOR_S
                                            "webkitgtk-"WEBKITGTK_API_VERSION_STRING""G_DIR_SEPARATOR_S
                                            "webinspector"G_DIR_SEPARATOR_S;
    return inspectorFilesPath;
}
