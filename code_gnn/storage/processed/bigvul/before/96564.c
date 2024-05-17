int mbedtls_ecp_check_pubkey( const mbedtls_ecp_group *grp, const mbedtls_ecp_point *pt )
{
     
    if( mbedtls_mpi_cmp_int( &pt->Z, 1 ) != 0 )
        return( MBEDTLS_ERR_ECP_INVALID_KEY );

#if defined(ECP_MONTGOMERY)
    if( ecp_get_type( grp ) == ECP_TYPE_MONTGOMERY )
        return( ecp_check_pubkey_mx( grp, pt ) );
#endif
#if defined(ECP_SHORTWEIERSTRASS)
    if( ecp_get_type( grp ) == ECP_TYPE_SHORT_WEIERSTRASS )
        return( ecp_check_pubkey_sw( grp, pt ) );
#endif
    return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );
}