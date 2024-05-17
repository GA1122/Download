static int ecp_check_pubkey_sw( const mbedtls_ecp_group *grp, const mbedtls_ecp_point *pt )
{
    int ret;
    mbedtls_mpi YY, RHS;

     
    if( mbedtls_mpi_cmp_int( &pt->X, 0 ) < 0 ||
        mbedtls_mpi_cmp_int( &pt->Y, 0 ) < 0 ||
        mbedtls_mpi_cmp_mpi( &pt->X, &grp->P ) >= 0 ||
        mbedtls_mpi_cmp_mpi( &pt->Y, &grp->P ) >= 0 )
        return( MBEDTLS_ERR_ECP_INVALID_KEY );

    mbedtls_mpi_init( &YY ); mbedtls_mpi_init( &RHS );

     
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mpi( &YY,  &pt->Y,   &pt->Y  ) );  MOD_MUL( YY  );
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mpi( &RHS, &pt->X,   &pt->X  ) );  MOD_MUL( RHS );

     
    if( grp->A.p == NULL )
    {
        MBEDTLS_MPI_CHK( mbedtls_mpi_sub_int( &RHS, &RHS, 3       ) );  MOD_SUB( RHS );
    }
    else
    {
        MBEDTLS_MPI_CHK( mbedtls_mpi_add_mpi( &RHS, &RHS, &grp->A ) );  MOD_ADD( RHS );
    }

    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mpi( &RHS, &RHS,     &pt->X  ) );  MOD_MUL( RHS );
    MBEDTLS_MPI_CHK( mbedtls_mpi_add_mpi( &RHS, &RHS,     &grp->B ) );  MOD_ADD( RHS );

    if( mbedtls_mpi_cmp_mpi( &YY, &RHS ) != 0 )
        ret = MBEDTLS_ERR_ECP_INVALID_KEY;

cleanup:

    mbedtls_mpi_free( &YY ); mbedtls_mpi_free( &RHS );

    return( ret );
}