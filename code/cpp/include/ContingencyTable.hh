#ifndef CONTINGENCY_TABLE_HH__
#define CONTINGENCY_TABLE_HH__

#include "LookupTable.hh"

#include <iosfwd>
#include <ostream>

#include <boost/math/distributions/chi_squared.hpp>

/**
  @class ContingencyTable
  @brief Models a (partially) filled contingency table

  Maintains a (partially) filled contingency table. Each table stores
  its corresponding distance threshold, making it possible to perform
  updates efficiently.
*/

class ContingencyTable
{
public:

  /** Creates an empty (invalid) contingency table */
  ContingencyTable();

  /**
    Creates a new contingency table that is empty. The distance
    threshold as well as the total number of instances of class
    1 are stored.

    The distance threshold is required for subsequent updates.

    The flag `withPseudocounts` changes the behaviour of the class. If set, the
    table will be created such that no cell can have a value of zero. This will
    ensure that the $p$-value is always valid.
  */

  ContingencyTable( unsigned n, unsigned n1, double threshold, bool withPseudocounts );

  /**
    Updates the contingency table by inserting a new element with
    a given distance and a given label. Exactly one entry will be
    changed by this procedure.
  */

  void insert( double distance, bool label );

  /**
    Checks whether two contingency tables are equal. Notice that this
    only involves checking that the values they hold are equal. There
    is no checking of the thresholds involved.
  */
  bool operator==( const ContingencyTable& other ) const noexcept;

  // Marginals ---------------------------------------------------------

  unsigned n()  const noexcept { return _as + _bs + _cs + _ds; }
  unsigned n1() const noexcept;
  unsigned n0() const noexcept;
  unsigned rs() const noexcept;
  unsigned qs() const noexcept;

  // $p$-values --------------------------------------------------------

  /**
    Calculates the $p$-value of a filled contingency table. This
    function does not work if the table is only partially filled
    or its values do not add up to the number of instances.
  */

  long double p() const;

  /**
    Calculates the minimum attainable $p$-value of a filled contingency
    table. This function does not work if the table is not fully filled
    or its values do not add up to the number of instances.
  */

  long double min_attainable_p() const;
  long double min_attainable_p( unsigned rs ) const;

  /**
    Calculates the minimum optimistic $p$-value of an incomplete table,
    i.e. the minimum $p$-value that can be attained if every remaining
    element is classified correctly.
  */

  long double min_optimistic_p() const;

  // Attributes --------------------------------------------------------

  unsigned as() const noexcept { return _as; }
  unsigned bs() const noexcept { return _bs; }
  unsigned cs() const noexcept { return _cs; }
  unsigned ds() const noexcept { return _ds; }

  double threshold() const noexcept { return _threshold; }

  /**
    Checks whether the table is *complete*, i.e. whether the marginals
    sum up to the expected values.
  */

  bool complete() const noexcept;

private:

  /**
    Calculates the Chi--squared test statistic of the given table
    and returns it. This is required for $p$-value calculation.
  */

  long double t() const;

  static boost::math::chi_squared_distribution<long double> _chi2;
  static LookupTable _lookupTable;

  unsigned _n;  //< Total number of items (fixed)
  unsigned _n1; //< Total number of items with class 1 (fixed)
  unsigned _n0; //< Total number of items with class 0 (fixed)

  // Entries in the actual contingency table. We follow common
  // terminology here, so that the table looks like this:
  //
  // ----------------
  // | as | bs | n1 |
  // |----|----|----|
  // | ds | cs | n0 |
  // |----|----|----|
  // | rs | qs | n  |
  // ----------------
  //
  // The marginals rs and qs are determined automatically upon
  // updating the table.
  unsigned _as;
  unsigned _bs;
  unsigned _cs;
  unsigned _ds;

  // Distance threshold for which this table was created. This value
  // decides where new entries will be added.
  double _threshold;
};

/**
  Output operator for a contingency table. The table will be reported as
  a list of numbers, i.e. `a, b, d, c`. This corresponds to the order of
  columns in the table.
*/

std::ostream& operator<<( std::ostream& o, const ContingencyTable& C );

#endif
