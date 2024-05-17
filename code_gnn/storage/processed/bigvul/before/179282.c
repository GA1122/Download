 static ext3_fsblk_t get_sb_block(void **data, struct super_block *sb)
 {
 	ext3_fsblk_t	sb_block;
 	char		*options = (char *) *data;
 
 	if (!options || strncmp(options, "sb=", 3) != 0)
 		return 1;	 
 	options += 3;
  	 
  	sb_block = simple_strtoul(options, &options, 0);
  	if (*options && *options != ',') {
		ext3_msg(sb, "error: invalid sb specification: %s",
// 		ext3_msg(sb, KERN_ERR, "error: invalid sb specification: %s",
  		       (char *) *data);
  		return 1;
  	}
 	if (*options == ',')
 		options++;
 	*data = (void *) options;
 	return sb_block;
 }