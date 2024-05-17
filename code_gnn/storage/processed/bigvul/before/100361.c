void BrowserRenderProcessHost::AddWord(const string16& word) {
  base::Thread* io_thread = g_browser_process->io_thread();
  SpellChecker* spellchecker = profile()->GetSpellChecker();
  if (spellchecker) {
    io_thread->message_loop()->PostTask(FROM_HERE, NewRunnableMethod(
        spellchecker, &SpellChecker::AddWord, word));
  }
}
