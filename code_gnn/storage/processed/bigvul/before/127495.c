static ColorProfile readColorProfile(jpeg_decompress_struct* info)
{
#if USE(ICCJPEG)
    JOCTET* profile;
    unsigned int profileLength;

    if (!read_icc_profile(info, &profile, &profileLength))
        return ColorProfile();

    bool ignoreProfile = false;
    char* profileData = reinterpret_cast<char*>(profile);
    if (profileLength < ImageDecoder::iccColorProfileHeaderLength)
        ignoreProfile = true;
    else if (!ImageDecoder::rgbColorProfile(profileData, profileLength))
        ignoreProfile = true;
    else if (!ImageDecoder::inputDeviceColorProfile(profileData, profileLength))
        ignoreProfile = true;

    ColorProfile colorProfile;
    if (!ignoreProfile)
        colorProfile.append(profileData, profileLength);
    free(profile);
    return colorProfile;
#else
    UNUSED_PARAM(info);
    return ColorProfile();
#endif
}
