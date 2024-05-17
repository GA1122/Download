int SoundPool::beginThread(void* arg)
{
 SoundPool* p = (SoundPool*)arg;
 return p->run();
}
