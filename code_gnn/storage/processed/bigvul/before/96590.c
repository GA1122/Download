int mbedtls_ecp_tls_read_point( const mbedtls_ecp_group *grp, mbedtls_ecp_point *pt,
                        const unsigned char **buf, size_t buf_len )
{
    unsigned char data_len;
    const unsigned char *buf_start;

     
    if( buf_len < 2 )
        return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

    data_len = *(*buf)++;
    if( data_len < 1 || data_len > buf_len - 1 )
        return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

     
    buf_start = *buf;
    *buf += data_len;

    return mbedtls_ecp_point_read_binary( grp, pt, buf_start, data_len );
}