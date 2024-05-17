 int mbedtls_ecp_gen_keypair_base( mbedtls_ecp_group *grp,
                     const mbedtls_ecp_point *G,
                     mbedtls_mpi *d, mbedtls_ecp_point *Q,
// int mbedtls_ecp_gen_privkey( const mbedtls_ecp_group *grp,
//                      mbedtls_mpi *d,
                       int (*f_rng)(void *, unsigned char *, size_t),
                       void *p_rng )
  {
    int ret;
//     int ret = MBEDTLS_ERR_ECP_BAD_INPUT_DATA;
      size_t n_size = ( grp->nbits + 7 ) / 8;
  
  #if defined(ECP_MONTGOMERY)
     if( ecp_get_type( grp ) == ECP_TYPE_MONTGOMERY )
     {
          
         size_t b;
 
         do {
             MBEDTLS_MPI_CHK( mbedtls_mpi_fill_random( d, n_size, f_rng, p_rng ) );
         } while( mbedtls_mpi_bitlen( d ) == 0);
 
          
         b = mbedtls_mpi_bitlen( d ) - 1;  
         if( b > grp->nbits )
             MBEDTLS_MPI_CHK( mbedtls_mpi_shift_r( d, b - grp->nbits ) );
         else
             MBEDTLS_MPI_CHK( mbedtls_mpi_set_bit( d, grp->nbits, 1 ) );
 
          
         MBEDTLS_MPI_CHK( mbedtls_mpi_set_bit( d, 0, 0 ) );
          MBEDTLS_MPI_CHK( mbedtls_mpi_set_bit( d, 1, 0 ) );
          MBEDTLS_MPI_CHK( mbedtls_mpi_set_bit( d, 2, 0 ) );
      }
    else
  #endif  
// 
  #if defined(ECP_SHORTWEIERSTRASS)
      if( ecp_get_type( grp ) == ECP_TYPE_SHORT_WEIERSTRASS )
      {
          
         int count = 0;
 
          
         do
         {
             MBEDTLS_MPI_CHK( mbedtls_mpi_fill_random( d, n_size, f_rng, p_rng ) );
             MBEDTLS_MPI_CHK( mbedtls_mpi_shift_r( d, 8 * n_size - grp->nbits ) );
 
              
             if( ++count > 30 )
                 return( MBEDTLS_ERR_ECP_RANDOM_FAILED );
         }
          while( mbedtls_mpi_cmp_int( d, 1 ) < 0 ||
                 mbedtls_mpi_cmp_mpi( d, &grp->N ) >= 0 );
      }
    else
  #endif  
        return( MBEDTLS_ERR_ECP_BAD_INPUT_DATA );
  
  cleanup:
    if( ret != 0 )
        return( ret );
//     return( ret );
// }
//  
  
    return( mbedtls_ecp_mul( grp, Q, d, G, f_rng, p_rng ) );
// int mbedtls_ecp_gen_keypair_base( mbedtls_ecp_group *grp,
//                      const mbedtls_ecp_point *G,
//                      mbedtls_mpi *d, mbedtls_ecp_point *Q,
//                      int (*f_rng)(void *, unsigned char *, size_t),
//                      void *p_rng )
// {
//     int ret;
// 
//     MBEDTLS_MPI_CHK( mbedtls_ecp_gen_privkey( grp, d, f_rng, p_rng ) );
//     MBEDTLS_MPI_CHK( mbedtls_ecp_mul( grp, Q, d, G, f_rng, p_rng ) );
// 
// cleanup:
//     return( ret );
  }