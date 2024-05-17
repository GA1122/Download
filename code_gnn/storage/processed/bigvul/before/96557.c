static int ecp_precompute_comb( const mbedtls_ecp_group *grp,
                                mbedtls_ecp_point T[], const mbedtls_ecp_point *P,
                                unsigned char w, size_t d )
{
    int ret;
    unsigned char i, k;
    size_t j;
    mbedtls_ecp_point *cur, *TT[COMB_MAX_PRE - 1];

     
    MBEDTLS_MPI_CHK( mbedtls_ecp_copy( &T[0], P ) );

    k = 0;
    for( i = 1; i < ( 1U << ( w - 1 ) ); i <<= 1 )
    {
        cur = T + i;
        MBEDTLS_MPI_CHK( mbedtls_ecp_copy( cur, T + ( i >> 1 ) ) );
        for( j = 0; j < d; j++ )
            MBEDTLS_MPI_CHK( ecp_double_jac( grp, cur, cur ) );

        TT[k++] = cur;
    }

    MBEDTLS_MPI_CHK( ecp_normalize_jac_many( grp, TT, k ) );

     
    k = 0;
    for( i = 1; i < ( 1U << ( w - 1 ) ); i <<= 1 )
    {
        j = i;
        while( j-- )
        {
            MBEDTLS_MPI_CHK( ecp_add_mixed( grp, &T[i + j], &T[j], &T[i] ) );
            TT[k++] = &T[i + j];
        }
    }

    MBEDTLS_MPI_CHK( ecp_normalize_jac_many( grp, TT, k ) );

cleanup:

    return( ret );
}