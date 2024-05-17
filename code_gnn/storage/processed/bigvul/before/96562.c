int mbedtls_ecp_check_privkey( const mbedtls_ecp_group *grp, const mbedtls_mpi *d )
{
#if defined(ECP_MONTGOMERY)
    if( ecp_get_type( grp ) == ECP_TYPE_MONTGOMERY )
    {
         
        if( mbedtls_mpi_get_bit( d, 0 ) != 0 ||
            mbedtls_mpi_get_bit( d, 1 ) != 0 ||
            mbedtls_mpi_get_bit( d, 2 ) != 0 ||
            mbedtls_mpi_bitlen( d ) - 1 != grp->nbits )  
            return( MBEDTLS_ERR_ECP_INVALID_KEY );
        else
            return( 0 );
    }
#endif  
#if defined(ECP_SHORTWEIERSTRASS)
    if( ecp_get_type( grp ) == ECP_TYPE_SHORT_WEIERSTRASS )
    {
         
        if( mbedtls_mpi_cmp_int( d, 1 ) < 0 ||
            mbedtls_mpi_cmp_mpi( d, &grp->N ) >= 0 )
            return( MBEDTLS_ERR_ECP_INVALID_KEY );
        else
            return( 0 );
    }
#endif  

    return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );
 }
