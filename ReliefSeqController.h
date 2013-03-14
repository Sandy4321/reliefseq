/**
 * \class ReliefSeqController
 *
 * \brief Interface for running ReliefSeq algorithms.
 */

#ifndef RELIEFSEQCONTROLLER_H
#define	RELIEFSEQCONTROLLER_H

#include <vector>

#include <boost/program_options.hpp>

#include "AttributeRanker.h"
#include "Dataset.h"
#include "Insilico.h"

namespace po = boost::program_options;

class ReliefSeqController {
public:
  /*************************************************************************//**
	 * Construct an ReliefSeqController object.
	 * \param [in] ds pointer to a Dataset object
	 * \param [in] vm reference to a Boost map of command line options
	 * \param [in] anaType analysis type
	 ****************************************************************************/
  ReliefSeqController(Dataset* ds, po::variables_map& vm,
          AnalysisType anaType = SNP_ONLY_ANALYSIS);
  // you know what you are
  virtual ~ReliefSeqController();
  /// Compute the scores based on the current set of attributes.
  bool ComputeScores();
  /// Compute scores based on optimum k
  bool ComputeScoresKopt();
  /// Get the last computed scores.
  AttributeScores& GetScores();
  /// Return the algorithm mode.
  std::string GetAlgorithmMode();
  /*************************************************************************//**
	 * Write the scores and attribute names to file.
	 * \param [in] baseFilename filename to write score-attribute name pairs
	 ****************************************************************************/
  void WriteAttributeScores(std::string baseFilename);
  /*************************************************************************//**
	 * Write the best k-nearest neighbors best k and attribute names to file.
	 * \param [in] baseFilename filename to write best-k-attribute name pairs
	 ****************************************************************************/
  void WriteBestKs(std::string baseFilename);
  /*************************************************************************//**
	 * Write the scores and attribute names to stream.
	 * \param [in] outStream stream to write score-attribute name pairs
	 ****************************************************************************/
  void PrintAttributeScores(std::ofstream& outStream);
  void PrintScores();
  void PrintBestKs();
private:
  /// Run the ReliefF algorithm.
  bool RunReliefF();
  /*************************************************************************//**
	 * Remove the worst attribute based on free energy scores.
	 * \param [in] numToRemove number of attributes to remove/evaporate
	 * \return distance
	 ****************************************************************************/
  bool RemoveWorstAttributes(unsigned int numToRemove = 1);
  /// Set the k-optimization parameters from the command line.
  bool SetKoptParameters();
  /// Determine the maximum k value for optimization.
  unsigned int GetKmax();
  
  /// pointer to a Dataset object
  Dataset* dataset;
  /// command line parameters map
  po::variables_map paramsMap;
  /// prefix for all output files
  std::string outFilesPrefix;

  /// type of analysis to perform
  /// \sa ReliefF
  AnalysisType analysisType;
  /// algorithm steps to perform
  std::string algorithmMode;

  /// pointer to an interaction ranker algorithm object
  AttributeRanker* reliefseqAlgorithm;

  // number of threads to use for random jungle
  unsigned int numThreads;
  /// number of attributes to remove per iteration
  unsigned int numToRemovePerIteration;
  /// number of attributes to remove next iteration
  unsigned int numToRemoveNextIteration;

  /// number of target attributes
  unsigned int numTargetAttributes;
  /// attributes that have been evaporated so far
  AttributeScores removedAttributes;
  /// current set of scores
  AttributeScores scores;

  /// optimize k begin value
  unsigned int koptBegin;
  /// optimize k end value
  unsigned int koptEnd;
  /// optimize k step value
  unsigned int koptStep;
  /// best k by attribute
  std::map<std::string, unsigned int> bestKs;
};

#endif
