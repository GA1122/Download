static unsigned append_symbol_coins(Coin* coins, const unsigned* frequencies, unsigned numcodes, size_t sum)
{
  unsigned i;
  unsigned j = 0;  
  for(i = 0; i < numcodes; i++)
  {
    if(frequencies[i] != 0)  
    {
      coins[j].weight = frequencies[i] / (float)sum;
      uivector_push_back(&coins[j].symbols, i);
      j++;
    }
  }
  return 0;
}
