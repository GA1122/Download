_gcry_mpi_powm (gcry_mpi_t res,
                gcry_mpi_t base, gcry_mpi_t expo, gcry_mpi_t mod)
{
   
  mpi_ptr_t  rp, ep, mp, bp;
  mpi_size_t esize, msize, bsize, rsize;
  int               msign, bsign, rsign;
   
  int        esec,  msec,  bsec;
   
  mpi_size_t size;
   
  int mod_shift_cnt;
  int negative_result;
  mpi_ptr_t mp_marker = NULL;
  mpi_ptr_t bp_marker = NULL;
  mpi_ptr_t ep_marker = NULL;
  mpi_ptr_t xp_marker = NULL;
  unsigned int mp_nlimbs = 0;
  unsigned int bp_nlimbs = 0;
  unsigned int ep_nlimbs = 0;
  unsigned int xp_nlimbs = 0;
  mpi_ptr_t tspace = NULL;
  mpi_size_t tsize = 0;


  esize = expo->nlimbs;
  msize = mod->nlimbs;
  size = 2 * msize;
  msign = mod->sign;

  esec = mpi_is_secure(expo);
  msec = mpi_is_secure(mod);
  bsec = mpi_is_secure(base);

  rp = res->d;
  ep = expo->d;
  MPN_NORMALIZE(ep, esize);

  if (!msize)
    _gcry_divide_by_zero();

  if (!esize)
    {
       
      res->nlimbs = (msize == 1 && mod->d[0] == 1) ? 0 : 1;
      if (res->nlimbs)
        {
          RESIZE_IF_NEEDED (res, 1);
          rp = res->d;
          rp[0] = 1;
        }
      res->sign = 0;
      goto leave;
    }

   
  mp_nlimbs = msec? msize:0;
  mp = mp_marker = mpi_alloc_limb_space(msize, msec);
  count_leading_zeros (mod_shift_cnt, mod->d[msize-1]);
  if (mod_shift_cnt)
    _gcry_mpih_lshift (mp, mod->d, msize, mod_shift_cnt);
  else
    MPN_COPY( mp, mod->d, msize );

  bsize = base->nlimbs;
  bsign = base->sign;
  if (bsize > msize)
    {
       
      bp_nlimbs = bsec ? (bsize + 1):0;
      bp = bp_marker = mpi_alloc_limb_space( bsize + 1, bsec );
      MPN_COPY ( bp, base->d, bsize );
       
      _gcry_mpih_divrem( bp + msize, 0, bp, bsize, mp, msize );
      bsize = msize;
       
      MPN_NORMALIZE( bp, bsize );
    }
  else
    bp = base->d;

  if (!bsize)
    {
      res->nlimbs = 0;
      res->sign = 0;
      goto leave;
    }


   
  if ( rp == bp )
    {
       
      gcry_assert (!bp_marker);
      bp_nlimbs = bsec? bsize:0;
      bp = bp_marker = mpi_alloc_limb_space( bsize, bsec );
      MPN_COPY(bp, rp, bsize);
    }
  if ( rp == ep )
    {
       
      ep_nlimbs = esec? esize:0;
      ep = ep_marker = mpi_alloc_limb_space( esize, esec );
      MPN_COPY(ep, rp, esize);
    }
  if ( rp == mp )
    {
       
      gcry_assert (!mp_marker);
      mp_nlimbs = msec?msize:0;
      mp = mp_marker = mpi_alloc_limb_space( msize, msec );
      MPN_COPY(mp, rp, msize);
    }

   
  if (res->alloced < size)
    {
      mpi_resize (res, size);
      rp = res->d;
    }
  MPN_COPY ( rp, bp, bsize );
  rsize = bsize;
  rsign = 0;

   
  {
    mpi_size_t i;
    mpi_ptr_t xp;
    int c;
    mpi_limb_t e;
    mpi_limb_t carry_limb;
    struct karatsuba_ctx karactx;

    xp_nlimbs = msec? (2 * (msize + 1)):0;
    xp = xp_marker = mpi_alloc_limb_space( 2 * (msize + 1), msec );

    memset( &karactx, 0, sizeof karactx );
    negative_result = (ep[0] & 1) && bsign;

    i = esize - 1;
    e = ep[i];
    count_leading_zeros (c, e);
    e = (e << c) << 1;      
    c = BITS_PER_MPI_LIMB - 1 - c;

     
    for (;;)
      {
        while (c)
          {
            mpi_ptr_t tp;
            mpi_size_t xsize;

             
            if ( rsize < KARATSUBA_THRESHOLD )
              _gcry_mpih_sqr_n_basecase( xp, rp, rsize );
            else
              {
                if ( !tspace )
                  {
                    tsize = 2 * rsize;
                    tspace = mpi_alloc_limb_space( tsize, 0 );
                  }
                else if ( tsize < (2*rsize) )
                  {
                    _gcry_mpi_free_limb_space (tspace, 0);
                    tsize = 2 * rsize;
                    tspace = mpi_alloc_limb_space (tsize, 0 );
                  }
                _gcry_mpih_sqr_n (xp, rp, rsize, tspace);
              }

            xsize = 2 * rsize;
            if ( xsize > msize )
              {
                _gcry_mpih_divrem(xp + msize, 0, xp, xsize, mp, msize);
                xsize = msize;
              }

            tp = rp; rp = xp; xp = tp;
            rsize = xsize;

             
            if (esec || (mpi_limb_signed_t)e < 0)
              {
                 
                if( bsize < KARATSUBA_THRESHOLD )
                  _gcry_mpih_mul ( xp, rp, rsize, bp, bsize );
                else
                  _gcry_mpih_mul_karatsuba_case (xp, rp, rsize, bp, bsize,
                                                 &karactx);

                xsize = rsize + bsize;
                if ( xsize > msize )
                  {
                    _gcry_mpih_divrem(xp + msize, 0, xp, xsize, mp, msize);
                    xsize = msize;
                  }
              }
            if ( (mpi_limb_signed_t)e < 0 )
              {
                tp = rp; rp = xp; xp = tp;
                rsize = xsize;
              }
            e <<= 1;
            c--;
          }

        i--;
        if ( i < 0 )
          break;
        e = ep[i];
        c = BITS_PER_MPI_LIMB;
      }

     
    if ( mod_shift_cnt )
      {
        carry_limb = _gcry_mpih_lshift( res->d, rp, rsize, mod_shift_cnt);
        rp = res->d;
        if ( carry_limb )
          {
            rp[rsize] = carry_limb;
            rsize++;
          }
      }
    else if (res->d != rp)
      {
        MPN_COPY (res->d, rp, rsize);
        rp = res->d;
      }

    if ( rsize >= msize )
      {
        _gcry_mpih_divrem(rp + msize, 0, rp, rsize, mp, msize);
        rsize = msize;
      }

     
    if ( mod_shift_cnt )
      _gcry_mpih_rshift( rp, rp, rsize, mod_shift_cnt);
    MPN_NORMALIZE (rp, rsize);

    _gcry_mpih_release_karatsuba_ctx (&karactx );
  }

   
  if ( negative_result && rsize )
    {
      if ( mod_shift_cnt )
        _gcry_mpih_rshift( mp, mp, msize, mod_shift_cnt);
      _gcry_mpih_sub( rp, mp, msize, rp, rsize);
      rsize = msize;
      rsign = msign;
      MPN_NORMALIZE(rp, rsize);
    }
  gcry_assert (res->d == rp);
  res->nlimbs = rsize;
  res->sign = rsign;

 leave:
  if (mp_marker)
    _gcry_mpi_free_limb_space( mp_marker, mp_nlimbs );
  if (bp_marker)
    _gcry_mpi_free_limb_space( bp_marker, bp_nlimbs );
  if (ep_marker)
    _gcry_mpi_free_limb_space( ep_marker, ep_nlimbs );
  if (xp_marker)
    _gcry_mpi_free_limb_space( xp_marker, xp_nlimbs );
  if (tspace)
    _gcry_mpi_free_limb_space( tspace, 0 );
}
