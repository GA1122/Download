static int ecp_double_jac( const mbedtls_ecp_group *grp, mbedtls_ecp_point *R,
                           const mbedtls_ecp_point *P )
{
    int ret;
    mbedtls_mpi M, S, T, U;

#if defined(MBEDTLS_SELF_TEST)
    dbl_count++;
#endif

#if defined(MBEDTLS_ECP_DOUBLE_JAC_ALT)
    if ( mbedtls_internal_ecp_grp_capable( grp ) )
    {
        return mbedtls_internal_ecp_double_jac( grp, R, P );
    }
#endif  

    mbedtls_mpi_init( &M ); mbedtls_mpi_init( &S ); mbedtls_mpi_init( &T ); mbedtls_mpi_init( &U );

     
    if( grp->A.p == NULL )
    {
         
        MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mpi( &S,  &P->Z,  &P->Z   ) ); MOD_MUL( S );
        MBEDTLS_MPI_CHK( mbedtls_mpi_add_mpi( &T,  &P->X,  &S      ) ); MOD_ADD( T );
        MBEDTLS_MPI_CHK( mbedtls_mpi_sub_mpi( &U,  &P->X,  &S      ) ); MOD_SUB( U );
        MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mpi( &S,  &T,     &U      ) ); MOD_MUL( S );
        MBEDTLS_MPI_CHK( mbedtls_mpi_mul_int( &M,  &S,     3       ) ); MOD_ADD( M );
    }
    else
    {
         
        MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mpi( &S,  &P->X,  &P->X   ) ); MOD_MUL( S );
        MBEDTLS_MPI_CHK( mbedtls_mpi_mul_int( &M,  &S,     3       ) ); MOD_ADD( M );

         
        if( mbedtls_mpi_cmp_int( &grp->A, 0 ) != 0 )
        {
             
            MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mpi( &S,  &P->Z,  &P->Z   ) ); MOD_MUL( S );
            MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mpi( &T,  &S,     &S      ) ); MOD_MUL( T );
            MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mpi( &S,  &T,     &grp->A ) ); MOD_MUL( S );
            MBEDTLS_MPI_CHK( mbedtls_mpi_add_mpi( &M,  &M,     &S      ) ); MOD_ADD( M );
        }
    }

     
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mpi( &T,  &P->Y,  &P->Y   ) ); MOD_MUL( T );
    MBEDTLS_MPI_CHK( mbedtls_mpi_shift_l( &T,  1               ) ); MOD_ADD( T );
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mpi( &S,  &P->X,  &T      ) ); MOD_MUL( S );
    MBEDTLS_MPI_CHK( mbedtls_mpi_shift_l( &S,  1               ) ); MOD_ADD( S );

     
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mpi( &U,  &T,     &T      ) ); MOD_MUL( U );
    MBEDTLS_MPI_CHK( mbedtls_mpi_shift_l( &U,  1               ) ); MOD_ADD( U );

     
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mpi( &T,  &M,     &M      ) ); MOD_MUL( T );
    MBEDTLS_MPI_CHK( mbedtls_mpi_sub_mpi( &T,  &T,     &S      ) ); MOD_SUB( T );
    MBEDTLS_MPI_CHK( mbedtls_mpi_sub_mpi( &T,  &T,     &S      ) ); MOD_SUB( T );

     
    MBEDTLS_MPI_CHK( mbedtls_mpi_sub_mpi( &S,  &S,     &T      ) ); MOD_SUB( S );
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mpi( &S,  &S,     &M      ) ); MOD_MUL( S );
    MBEDTLS_MPI_CHK( mbedtls_mpi_sub_mpi( &S,  &S,     &U      ) ); MOD_SUB( S );

     
    MBEDTLS_MPI_CHK( mbedtls_mpi_mul_mpi( &U,  &P->Y,  &P->Z   ) ); MOD_MUL( U );
    MBEDTLS_MPI_CHK( mbedtls_mpi_shift_l( &U,  1               ) ); MOD_ADD( U );

    MBEDTLS_MPI_CHK( mbedtls_mpi_copy( &R->X, &T ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_copy( &R->Y, &S ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_copy( &R->Z, &U ) );

cleanup:
    mbedtls_mpi_free( &M ); mbedtls_mpi_free( &S ); mbedtls_mpi_free( &T ); mbedtls_mpi_free( &U );

    return( ret );
}