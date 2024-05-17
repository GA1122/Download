static bool checkExifHeader(jpeg_saved_marker_ptr marker, bool& isBigEndian, unsigned& ifdOffset)
{
    const unsigned exifHeaderSize = 14;
    if (!(marker->marker == exifMarker
        && marker->data_length >= exifHeaderSize
        && marker->data[0] == 'E'
        && marker->data[1] == 'x'
        && marker->data[2] == 'i'
        && marker->data[3] == 'f'
        && marker->data[4] == '\0'
        && ((marker->data[6] == 'I' && marker->data[7] == 'I')
            || (marker->data[6] == 'M' && marker->data[7] == 'M'))))
        return false;

    isBigEndian = marker->data[6] == 'M';
    if (readUint16(marker->data + 8, isBigEndian) != 42)
        return false;

    ifdOffset = readUint32(marker->data + 10, isBigEndian);
    return true;
}
