  static int opl3_load_patch(int dev, int format, const char __user *addr,
		int offs, int count, int pmgr_flag)
// 		int count, int pmgr_flag)
  {
  	struct sbi_instrument ins;
  
 	if (count <sizeof(ins))
 	{
 		printk(KERN_WARNING "FM Error: Patch record too short\n");
  		return -EINVAL;
  	}
  
	 
	if(copy_from_user(&((char *) &ins)[offs], addr + offs, sizeof(ins) - offs))
// 	if (copy_from_user(&ins, addr, sizeof(ins)))
  		return -EFAULT;
  
  	if (ins.channel < 0 || ins.channel >= SBFM_MAXINSTR)
 	{
 		printk(KERN_WARNING "FM Error: Invalid instrument number %d\n", ins.channel);
 		return -EINVAL;
 	}
 	ins.key = format;
 
 	return store_instr(ins.channel, &ins);
 }