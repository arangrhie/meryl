
/******************************************************************************
 *
 *  This file is part of 'sequence' and/or 'meryl', software programs for
 *  working with DNA sequence files and k-mers contained in them.
 *
 *  Modifications by:
 *
 *    Brian P. Walenz beginning on 2018-FEB-26
 *      are a 'United States Government Work', and
 *      are released in the public domain
 *
 *  File 'README.license' in the root directory of this distribution contains
 *  full conditions and disclaimers.
 */

#include "meryl.H"

void
merylOperation::reportHistogram(void) {

  if (_inputs.size() > 1)
    fprintf(stderr, "ERROR: told to dump a histogram for more than one input!\n"), exit(1);

  if (_inputs[0]->_operation)
    fprintf(stderr, "ERROR: told to dump a histogram from input '%s'!\n", _inputs[0]->_name), exit(1);

  if (_inputs[0]->_sequence)
    fprintf(stderr, "ERROR: told to dump a histogram from input '%s'!\n", _inputs[0]->_name), exit(1);

  //  Tell the stream to load and return the histogram.

  kmerCountStatistics *stats = _inputs[0]->_stream->stats();

  //  Now just dump it.

  for (uint32 frequency=1; frequency<stats->numFrequencies(); frequency++) {
    uint64 nkmers = stats->numKmersAtFrequency(frequency);

    if (nkmers > 0)
      fprintf(stdout, F_U32 "\t" F_U64 "\n", frequency, nkmers);
  }

  //for (uint32 ii=0; ii<stats->_hbigLen; ii++) {
  //}
}



void
merylOperation::reportStatistics(void) {

  if (_inputs.size() > 1)
    fprintf(stderr, "ERROR: told to dump a histogram for more than one input!\n"), exit(1);

  if (_inputs[0]->_operation)
    fprintf(stderr, "ERROR: told to dump a histogram from input '%s'!\n", _inputs[0]->_name), exit(1);

  if (_inputs[0]->_sequence)
    fprintf(stderr, "ERROR: told to dump a histogram from input '%s'!\n", _inputs[0]->_name), exit(1);

  //  Tell the stream to load and return the histogram.

  kmerCountStatistics *stats = _inputs[0]->_stream->stats();

  //  Now just dump it.

  uint64  nUniverse = uint64MASK(kmer::merSize() * 2) + 1;
  uint64  sDistinct = 0;
  uint64  sTotal    = 0;

  fprintf(stdout, "Number of %u-mers that are:\n", kmer::merSize());
  fprintf(stdout, "  unique   %20" F_U64P "  (exactly one instance of the kmer is in the input)\n", stats->numUnique());
  fprintf(stdout, "  distinct %20" F_U64P "  (non-redundant kmer sequences in the input)\n", stats->numDistinct());
  fprintf(stdout, "  present  %20" F_U64P "  (...)\n", stats->numTotal());
  fprintf(stdout, "  missing  %20" F_U64P "  (non-redundant kmer sequences not in the input)\n", nUniverse - stats->numDistinct());
  fprintf(stdout, "\n");
  fprintf(stdout, "             number of   cumulative   cumulative     presence\n");
  fprintf(stdout, "              distinct     fraction     fraction   in dataset\n");
  fprintf(stdout, "frequency        kmers     distinct        total       (1e-6)\n");
  fprintf(stdout, "--------- ------------ ------------ ------------ ------------\n");

  for (uint32 frequency=1; frequency<stats->numFrequencies(); frequency++) {
    uint64 nkmers = stats->numKmersAtFrequency(frequency);

    sDistinct  += nkmers;
    sTotal     += nkmers * frequency;

    if (nkmers > 0)
      fprintf(stdout, "%9" F_U32P " %12" F_U64P " %12.4f %12.4f %12.6f\n",
              frequency,
              nkmers,
              (double)sDistinct / stats->numDistinct(),
              (double)sTotal    / stats->numTotal(),
              (double)frequency / stats->numTotal() * 1000000.0);
  }

  assert(sDistinct == stats->numDistinct());
  assert(sTotal    == stats->numTotal());

  //for (uint32 ii=0; ii<stats->_hbigLen; ii++) {
  //}
}
