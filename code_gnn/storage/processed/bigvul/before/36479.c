static struct snd_kcontrol *snd_ctl_new(struct snd_kcontrol *control,
					unsigned int access)
{
	struct snd_kcontrol *kctl;
	unsigned int idx;
	
	if (snd_BUG_ON(!control || !control->count))
		return NULL;

	if (control->count > MAX_CONTROL_COUNT)
		return NULL;

	kctl = kzalloc(sizeof(*kctl) + sizeof(struct snd_kcontrol_volatile) * control->count, GFP_KERNEL);
	if (kctl == NULL) {
		pr_err("ALSA: Cannot allocate control instance\n");
		return NULL;
	}
	*kctl = *control;
	for (idx = 0; idx < kctl->count; idx++)
		kctl->vd[idx].access = access;
	return kctl;
}