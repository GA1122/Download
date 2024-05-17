divide (mpn_t a, mpn_t b, mpn_t *q)
{
   
  const mp_limb_t *a_ptr = a.limbs;
  size_t a_len = a.nlimbs;
  const mp_limb_t *b_ptr = b.limbs;
  size_t b_len = b.nlimbs;
  mp_limb_t *roomptr;
  mp_limb_t *tmp_roomptr = NULL;
  mp_limb_t *q_ptr;
  size_t q_len;
  mp_limb_t *r_ptr;
  size_t r_len;

   
  roomptr = (mp_limb_t *) malloc ((a_len + 2) * sizeof (mp_limb_t));
  if (roomptr == NULL)
    return NULL;

   
  while (a_len > 0 && a_ptr[a_len - 1] == 0)
    a_len--;

   
  for (;;)
    {
      if (b_len == 0)
         
        abort ();
      if (b_ptr[b_len - 1] == 0)
        b_len--;
      else
        break;
    }

   

  if (a_len < b_len)
    {
       
      r_ptr = roomptr;
      r_len = a_len;
      memcpy (r_ptr, a_ptr, a_len * sizeof (mp_limb_t));
      q_ptr = roomptr + a_len;
      q_len = 0;
    }
  else if (b_len == 1)
    {
       
      r_ptr = roomptr;
      q_ptr = roomptr + 1;
      {
        mp_limb_t den = b_ptr[0];
        mp_limb_t remainder = 0;
        const mp_limb_t *sourceptr = a_ptr + a_len;
        mp_limb_t *destptr = q_ptr + a_len;
        size_t count;
        for (count = a_len; count > 0; count--)
          {
            mp_twolimb_t num =
              ((mp_twolimb_t) remainder << GMP_LIMB_BITS) | *--sourceptr;
            *--destptr = num / den;
            remainder = num % den;
          }
         
        if (remainder > 0)
          {
            r_ptr[0] = remainder;
            r_len = 1;
          }
        else
          r_len = 0;
         
        q_len = a_len;
        if (q_ptr[q_len - 1] == 0)
          q_len--;
      }
    }
  else
    {
       
       
      size_t s;
      {
        mp_limb_t msd = b_ptr[b_len - 1];  
         
# if __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4)
        s = __builtin_clz (msd);
# else
#  if defined DBL_EXPBIT0_WORD && defined DBL_EXPBIT0_BIT
        if (GMP_LIMB_BITS <= DBL_MANT_BIT)
          {
             
#   define DBL_EXP_MASK ((DBL_MAX_EXP - DBL_MIN_EXP) | 7)
#   define DBL_EXP_BIAS (DBL_EXP_MASK / 2 - 1)
#   define NWORDS \
     ((sizeof (double) + sizeof (unsigned int) - 1) / sizeof (unsigned int))
            union { double value; unsigned int word[NWORDS]; } m;

             
            m.value = msd;

            s = GMP_LIMB_BITS
                - (((m.word[DBL_EXPBIT0_WORD] >> DBL_EXPBIT0_BIT) & DBL_EXP_MASK)
                   - DBL_EXP_BIAS);
          }
        else
#   undef NWORDS
#  endif
          {
            s = 31;
            if (msd >= 0x10000)
              {
                msd = msd >> 16;
                s -= 16;
              }
            if (msd >= 0x100)
              {
                msd = msd >> 8;
                s -= 8;
              }
            if (msd >= 0x10)
              {
                msd = msd >> 4;
                s -= 4;
              }
            if (msd >= 0x4)
              {
                msd = msd >> 2;
                s -= 2;
              }
            if (msd >= 0x2)
              {
                msd = msd >> 1;
                s -= 1;
              }
          }
# endif
      }
       
      if (s > 0)
        {
          tmp_roomptr = (mp_limb_t *) malloc (b_len * sizeof (mp_limb_t));
          if (tmp_roomptr == NULL)
            {
              free (roomptr);
              return NULL;
            }
          {
            const mp_limb_t *sourceptr = b_ptr;
            mp_limb_t *destptr = tmp_roomptr;
            mp_twolimb_t accu = 0;
            size_t count;
            for (count = b_len; count > 0; count--)
              {
                accu += (mp_twolimb_t) *sourceptr++ << s;
                *destptr++ = (mp_limb_t) accu;
                accu = accu >> GMP_LIMB_BITS;
              }
             
            if (accu != 0)
              abort ();
          }
          b_ptr = tmp_roomptr;
        }
       
      r_ptr = roomptr;
      if (s == 0)
        {
          memcpy (r_ptr, a_ptr, a_len * sizeof (mp_limb_t));
          r_ptr[a_len] = 0;
        }
      else
        {
          const mp_limb_t *sourceptr = a_ptr;
          mp_limb_t *destptr = r_ptr;
          mp_twolimb_t accu = 0;
          size_t count;
          for (count = a_len; count > 0; count--)
            {
              accu += (mp_twolimb_t) *sourceptr++ << s;
              *destptr++ = (mp_limb_t) accu;
              accu = accu >> GMP_LIMB_BITS;
            }
          *destptr++ = (mp_limb_t) accu;
        }
      q_ptr = roomptr + b_len;
      q_len = a_len - b_len + 1;  
      {
        size_t j = a_len - b_len;  
        mp_limb_t b_msd = b_ptr[b_len - 1];  
        mp_limb_t b_2msd = b_ptr[b_len - 2];  
        mp_twolimb_t b_msdd =  
          ((mp_twolimb_t) b_msd << GMP_LIMB_BITS) | b_2msd;
         
        for (;;)
          {
            mp_limb_t q_star;
            mp_limb_t c1;
            if (r_ptr[j + b_len] < b_msd)  
              {
                 
                mp_twolimb_t num =
                  ((mp_twolimb_t) r_ptr[j + b_len] << GMP_LIMB_BITS)
                  | r_ptr[j + b_len - 1];
                q_star = num / b_msd;
                c1 = num % b_msd;
              }
            else
              {
                 
                q_star = (mp_limb_t)~(mp_limb_t)0;  
                 
                if (r_ptr[j + b_len] > b_msd
                    || (c1 = r_ptr[j + b_len - 1] + b_msd) < b_msd)
                   
                  goto subtract;
              }
             
            {
              mp_twolimb_t c2 =  
                ((mp_twolimb_t) c1 << GMP_LIMB_BITS) | r_ptr[j + b_len - 2];
              mp_twolimb_t c3 =  
                (mp_twolimb_t) b_2msd * (mp_twolimb_t) q_star;
               
              if (c3 > c2)
                {
                  q_star = q_star - 1;  
                  if (c3 - c2 > b_msdd)
                    q_star = q_star - 1;  
                }
            }
            if (q_star > 0)
              subtract:
              {
                 
                mp_limb_t cr;
                {
                  const mp_limb_t *sourceptr = b_ptr;
                  mp_limb_t *destptr = r_ptr + j;
                  mp_twolimb_t carry = 0;
                  size_t count;
                  for (count = b_len; count > 0; count--)
                    {
                       
                      carry =
                        carry
                        + (mp_twolimb_t) q_star * (mp_twolimb_t) *sourceptr++
                        + (mp_limb_t) ~(*destptr);
                       
                      *destptr++ = ~(mp_limb_t) carry;
                      carry = carry >> GMP_LIMB_BITS;  
                    }
                  cr = (mp_limb_t) carry;
                }
                 
                if (cr > r_ptr[j + b_len])
                  {
                     
                    q_star = q_star - 1;  
                     
                    {
                      const mp_limb_t *sourceptr = b_ptr;
                      mp_limb_t *destptr = r_ptr + j;
                      mp_limb_t carry = 0;
                      size_t count;
                      for (count = b_len; count > 0; count--)
                        {
                          mp_limb_t source1 = *sourceptr++;
                          mp_limb_t source2 = *destptr;
                          *destptr++ = source1 + source2 + carry;
                          carry =
                            (carry
                             ? source1 >= (mp_limb_t) ~source2
                             : source1 > (mp_limb_t) ~source2);
                        }
                    }
                     
                  }
              }
             
            q_ptr[j] = q_star;
            if (j == 0)
              break;
            j--;
          }
      }
      r_len = b_len;
       
      if (q_ptr[q_len - 1] == 0)
        q_len--;
# if 0  
       
      if (s > 0)
        {
          mp_limb_t ptr = r_ptr + r_len;
          mp_twolimb_t accu = 0;
          size_t count;
          for (count = r_len; count > 0; count--)
            {
              accu = (mp_twolimb_t) (mp_limb_t) accu << GMP_LIMB_BITS;
              accu += (mp_twolimb_t) *--ptr << (GMP_LIMB_BITS - s);
              *ptr = (mp_limb_t) (accu >> GMP_LIMB_BITS);
            }
        }
# endif
       
      while (r_len > 0 && r_ptr[r_len - 1] == 0)
        r_len--;
    }
   
  if (r_len > b_len)
    goto increment_q;
  {
    size_t i;
    for (i = b_len;;)
      {
        mp_limb_t r_i =
          (i <= r_len && i > 0 ? r_ptr[i - 1] >> (GMP_LIMB_BITS - 1) : 0)
          | (i < r_len ? r_ptr[i] << 1 : 0);
        mp_limb_t b_i = (i < b_len ? b_ptr[i] : 0);
        if (r_i > b_i)
          goto increment_q;
        if (r_i < b_i)
          goto keep_q;
        if (i == 0)
          break;
        i--;
      }
  }
  if (q_len > 0 && ((q_ptr[0] & 1) != 0))
     
    increment_q:
    {
      size_t i;
      for (i = 0; i < q_len; i++)
        if (++(q_ptr[i]) != 0)
          goto keep_q;
      q_ptr[q_len++] = 1;
    }
  keep_q:
  if (tmp_roomptr != NULL)
    free (tmp_roomptr);
  q->limbs = q_ptr;
  q->nlimbs = q_len;
  return roomptr;
}
