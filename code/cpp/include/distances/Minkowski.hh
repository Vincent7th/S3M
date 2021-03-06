#ifndef DISTANCES_MINKOWSKI_HH__
#define DISTANCES_MINKOWSKI_HH__

#include "DistanceFunctor.hh"

#include <sstream>

class MinkowskiDistance : public DistanceFunctor
{
public:
  using ValueType = DistanceFunctor::ValueType;

  MinkowskiDistance( ValueType p );

  virtual ValueType operator()( const TimeSeries& S, const TimeSeries& T ) const;

  virtual std::string name() const noexcept
  {
    // In contrast to `std::to_string`, this uses the *default*
    // notation for output streams, so that we get '0', instead
    // of '0.000000'.
    std::ostringstream stream;
    stream << _p;

    return "Minkowski:" + stream.str();
  }

private:
  ValueType _p;
};

#endif
