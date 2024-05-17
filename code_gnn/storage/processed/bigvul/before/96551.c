static int ecp_mul_comb( mbedtls_ecp_group *grp, mbedtls_ecp_point *R,
                         const mbedtls_mpi *m, const mbedtls_ecp_point *P,
                         int (*f_rng)(void *, unsigned char *, size_t),
                         void *p_rng )
{
    int ret;
    unsigned char w, m_is_odd, p_eq_g, pre_len, i;
    size_t d;
    unsigned char k[COMB_MAX_D + 1];
    mbedtls_ecp_point *T;
    mbedtls_mpi M, mm;

    mbedtls_mpi_init( &M );
    mbedtls_mpi_init( &mm );

     
    if( mbedtls_mpi_get_bit( &grp->N, 0 ) != 1 )
        return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );

     
    w = grp->nbits >= 384 ? 5 : 4;

     
#if MBEDTLS_ECP_FIXED_POINT_OPTIM == 1
    p_eq_g = ( mbedtls_mpi_cmp_mpi( &P->Y, &grp->G.Y ) == 0 &&
               mbedtls_mpi_cmp_mpi( &P->X, &grp->G.X ) == 0 );
    if( p_eq_g )
        w++;
#else
    p_eq_g = 0;
#endif

     
    if( w > MBEDTLS_ECP_WINDOW_SIZE )
        w = MBEDTLS_ECP_WINDOW_SIZE;
    if( w >= grp->nbits )
        w = 2;

     
    pre_len = 1U << ( w - 1 );
    d = ( grp->nbits + w - 1 ) / w;

     
    T = p_eq_g ? grp->T : NULL;

    if( T == NULL )
    {
        T = mbedtls_calloc( pre_len, sizeof( mbedtls_ecp_point ) );
        if( T == NULL )
        {
            ret = MBEDTLS_ERR_ECP_ALLOC_FAILED;
            goto cleanup;
        }

        MBEDTLS_MPI_CHK( ecp_precompute_comb( grp, T, P, w, d ) );

        if( p_eq_g )
        {
            grp->T = T;
            grp->T_size = pre_len;
        }
    }

     
    m_is_odd = ( mbedtls_mpi_get_bit( m, 0 ) == 1 );
    MBEDTLS_MPI_CHK( mbedtls_mpi_copy( &M, m ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_sub_mpi( &mm, &grp->N, m ) );
    MBEDTLS_MPI_CHK( mbedtls_mpi_safe_cond_assign( &M, &mm, ! m_is_odd ) );

     
    ecp_comb_fixed( k, d, w, &M );
    MBEDTLS_MPI_CHK( ecp_mul_comb_core( grp, R, T, pre_len, k, d, f_rng, p_rng ) );

     
    MBEDTLS_MPI_CHK( ecp_safe_invert_jac( grp, R, ! m_is_odd ) );
    MBEDTLS_MPI_CHK( ecp_normalize_jac( grp, R ) );

cleanup:

     
    if( T != NULL && T != grp->T )
    {
        for( i = 0; i < pre_len; i++ )
            mbedtls_ecp_point_free( &T[i] );
        mbedtls_free( T );
    }

    mbedtls_mpi_free( &M );
    mbedtls_mpi_free( &mm );

    if( ret != 0 )
        mbedtls_ecp_point_free( R );

    return( ret );
}
