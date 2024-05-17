	__releases(key_user_lock)
{
	spin_unlock(&key_user_lock);
}
