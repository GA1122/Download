nsPluginInstance::DestroyStream(NPStream*  , NPError  )
{
    if (_streamfd != -1) {
        if (close(_streamfd) == -1) {
            perror("closing _streamfd");
        } else {
            _streamfd = -1;
        }
    }

    return NPERR_NO_ERROR;
}
