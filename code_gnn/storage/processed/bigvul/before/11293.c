static void delete_unneeded_choice_fields(asn1_node p)
{
  asn1_node p2;

  while (p->right)
    {
      p2 = p->right;
      asn1_delete_structure (&p2);
    }
}
