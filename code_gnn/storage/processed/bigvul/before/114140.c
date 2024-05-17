quit(Eina_Bool success, const char *msg)
{
    ewk_shutdown();
    elm_shutdown();

    if (msg)
        fputs(msg, (success) ? stdout : stderr);

    if (!success)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
