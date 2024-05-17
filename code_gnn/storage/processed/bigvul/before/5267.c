bmexec (kwset_t kwset, char const *text, size_t size)
{
   
  return (kwset->trans
          ? bmexec_trans (kwset, text, size)
          : bmexec_trans (kwset, text, size));
}
