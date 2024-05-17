unsigned int gsicc_getprofilesize(unsigned char *buffer)
{
    return ( (buffer[0] << 24) + (buffer[1] << 16) +
             (buffer[2] << 8)  +  buffer[3] );
}
