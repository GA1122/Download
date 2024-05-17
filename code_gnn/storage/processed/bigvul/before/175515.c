WORD32 ihevcd_extend_sign_bit(WORD32 value, WORD32 num_bits)
{
    WORD32 ret_value = value;
 if(value >> (num_bits - 1))
 {
        ret_value |= (0xFFFFFFFF << num_bits);
 }
 return ret_value;
}
