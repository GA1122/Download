int snd_pcm_hw_constraint_ratnums(struct snd_pcm_runtime *runtime, 
				  unsigned int cond,
				  snd_pcm_hw_param_t var,
				  const struct snd_pcm_hw_constraint_ratnums *r)
{
	return snd_pcm_hw_rule_add(runtime, cond, var,
				   snd_pcm_hw_rule_ratnums, (void *)r,
				   var, -1);
}