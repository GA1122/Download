H264SwDecApiVersion H264SwDecGetAPIVersion()
{
    H264SwDecApiVersion ver;

    ver.major = H264SWDEC_MAJOR_VERSION;
    ver.minor = H264SWDEC_MINOR_VERSION;

 return(ver);
}
