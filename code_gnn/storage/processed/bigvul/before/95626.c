static void S_AL_AddLoopingSound(int entityNum, const vec3_t origin, const vec3_t velocity, const int range, sfxHandle_t sfx, int volume)
{
	S_AL_SrcLoop(SRCPRI_ENTITY, sfx, origin, velocity, entityNum, volume);
}
