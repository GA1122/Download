Time XRRConfigTimes (XRRScreenConfiguration *config, Time *config_timestamp)
{
    *config_timestamp = config->config_timestamp;
    return config->timestamp;
}
