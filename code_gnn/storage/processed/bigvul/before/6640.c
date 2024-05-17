AviaryScheddPlugin::handleTransportSocket(Stream *)
{
    string provider_error;
    if (!provider->processRequest(provider_error)) {
        dprintf (D_ALWAYS,"Error processing request: %s\n",provider_error.c_str());
    }

    return KEEP_STREAM;
}
