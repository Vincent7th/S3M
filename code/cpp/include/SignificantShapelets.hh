#ifndef SIGNIFICANT_SHAPELETS_HH__
#define SIGNIFICANT_SHAPELETS_HH__

#include "ContingencyTable.hh"
#include "TimeSeries.hh"

#include <initializer_list>
#include <iosfwd>
#include <vector>

/**
  @class SignificantShapelets
  @brief Main extraction class for significant shapelets

  This is the main class for performing the extraction of significant
  shapelets from a given set of time series. Its entry point is given
  by `operator()`, in which the extraction process is performed. Note
  that the implementation of `operator()` follows the pseudocode from
  the paper.
*/

class SignificantShapelets
{
public:

  using ValueType = typename TimeSeries::ValueType;

  /** Data structure for describing the result of the extraction process */
  struct SignificantShapelet
  {
    TimeSeries shapelet;    // the extracted shapelet
    long double p;          // $p$-value
    ContingencyTable table; // best contingency table
  };

  // Constructors ------------------------------------------------------

  /**
    Creates a new instance of a significant shapelet extractor with
    a fixed window size and stride for candidate generation.
  */

  SignificantShapelets( unsigned size, unsigned windowStride );

  /**
    Creates a new instances of a significant shapelet extractor with
    a predefined minimum/maximum window size and a window stride for
    candidate generation.
  */

  SignificantShapelets( unsigned minSize, unsigned maxSize, unsigned windowStride );

  // Parameters --------------------------------------------------------

  void defaultFactor( bool value = true ) noexcept
  {
    _defaultFactor = value;
  }

  void disablePruning( bool value = true ) noexcept
  {
    _disablePruning = value;
  }

  void keepNormalOnly( bool value = true ) noexcept
  {
    _keepNormalOnly = value;
  }

  void mergeTables( bool value = true ) noexcept
  {
    _mergeTables = value;
  }

  void removeDuplicates( bool value = true ) noexcept
  {
    _removeDuplicates = value;
  }

  void reportAllShapelets( bool value = true ) noexcept
  {
    _reportAllShapelets = value;

    // Reporting *all* shapelets, even the insignificant ones, implies
    // that no pruning may be performed.
    if( _reportAllShapelets )
      _disablePruning = true;
  }

  // Extraction --------------------------------------------------------

  /**
    Performs the extraction process with the given settings for a set of
    time series with a set of associated labels.
  */

  std::vector<SignificantShapelet> operator()( const std::vector<TimeSeries>& timeSeries,
                                               const std::vector<bool>& labels,
                                               long double& tarone,
                                               std::vector<long double>& thresholds );

private:

  /**
    Calculates the minimum attainable $p$-values for a given problem
    size. This is required in order to perform the adjustment of the
    $p$-value following Tarone's method.

    The returned values will be sorted in ascending order. Hence, it
    is required to access elements from the back of the vector.
  */

  static std::vector<long double> min_attainable_p_values( unsigned n, unsigned n1 );

  unsigned _minWindowSize;
  unsigned _maxWindowSize;
  unsigned _windowStride;

  bool _defaultFactor      = false;
  bool _disablePruning     = false;
  bool _keepNormalOnly     = false;
  bool _mergeTables        = false;
  bool _removeDuplicates   = false;
  bool _reportAllShapelets = false;

  // Target FWER before any adjustments of the threshold are being made
  // using Tarone's method.
  double _alpha = 0.01;
};

/**
  Adds a significant shapelet to an output stream. JSON is used to
  format the output.
*/

std::ostream& operator<<( std::ostream& o, const SignificantShapelets::SignificantShapelet& ss );

#endif
