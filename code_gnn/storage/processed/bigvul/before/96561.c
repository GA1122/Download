static int ecp_select_comb( const mbedtls_ecp_group *grp, mbedtls_ecp_point *R,
                            const mbedtls_ecp_point T[], unsigned char t_len,
                            unsigned char i )
{
    int ret;
    unsigned char ii, j;

     
    ii =  ( i & 0x7Fu ) >> 1;

     
    for( j = 0; j < t_len; j++ )
    {
        MBEDTLS_MPI_CHK( mbedtls_mpi_safe_cond_assign( &R->X, &T[j].X, j == ii ) );
        MBEDTLS_MPI_CHK( mbedtls_mpi_safe_cond_assign( &R->Y, &T[j].Y, j == ii ) );
    }

     
    MBEDTLS_MPI_CHK( ecp_safe_invert_jac( grp, R, i >> 7 ) );

cleanup:
    return( ret );
}