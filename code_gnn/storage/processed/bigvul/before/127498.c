static unsigned readUint32(JOCTET* data, bool isBigEndian)
{
    if (isBigEndian)
        return (GETJOCTET(data[0]) << 24) | (GETJOCTET(data[1]) << 16) | (GETJOCTET(data[2]) << 8) | GETJOCTET(data[3]);
    return (GETJOCTET(data[3]) << 24) | (GETJOCTET(data[2]) << 16) | (GETJOCTET(data[1]) << 8) | GETJOCTET(data[0]);
}
