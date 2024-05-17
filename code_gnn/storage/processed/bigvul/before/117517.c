snd_mixer_elem_t* AudioMixerAlsa::FindElementWithName(
    snd_mixer_t* handle, const string& element_name) const {
  DCHECK(MessageLoop::current() == thread_->message_loop());
  snd_mixer_selem_id_t* sid = NULL;

  if (snd_mixer_selem_id_malloc(&sid))
    return NULL;

  snd_mixer_selem_id_set_index(sid, 0);
  snd_mixer_selem_id_set_name(sid, element_name.c_str());
  snd_mixer_elem_t* element = snd_mixer_find_selem(handle, sid);
  if (!element)
    VLOG(1) << "Unable to find control " << snd_mixer_selem_id_get_name(sid);

  snd_mixer_selem_id_free(sid);
  return element;
}
