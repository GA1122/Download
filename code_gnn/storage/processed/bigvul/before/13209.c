nsPluginInstance::NewStream(NPMIMEType  , NPStream* stream,
                            NPBool  , uint16_t*  )
{

    if (_childpid) {
        return NPERR_GENERIC_ERROR;
    }
    _swf_url = stream->url;

    if (!_swf_url.empty() && _window) {
        return startProc();
    }

    return NPERR_NO_ERROR;
}
