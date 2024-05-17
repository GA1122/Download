read_4(struct file *file, png_uint_32 *pu)
  
{
 unsigned int i = 0;
   png_uint_32 val = 0;

 do
 {
 int ch = read_byte(file);

 if (ch == EOF)
 return i;

      val = (val << 8) + ch;
 } while (++i < 4);

 *pu = val;
 return i;
}
