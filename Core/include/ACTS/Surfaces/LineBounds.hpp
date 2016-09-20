// This file is part of the ACTS project.
//
// Copyright (C) 2016 ACTS project team
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

///////////////////////////////////////////////////////////////////
// LineBounds.h, ACTS project
///////////////////////////////////////////////////////////////////

#ifndef ACTS_SURFACES_LINEBOUNDS_H
#define ACTS_SURFACES_LINEBOUNDS_H 1

#include "ACTS/Surfaces/SurfaceBounds.hpp"
#include "ACTS/Utilities/Definitions.hpp"

namespace Acts {

/// @class LineBounds
///
/// Bounds for a LineSurface.
///

class LineBounds : public SurfaceBounds
{
public:
  /// @enum BoundValues for readablility
  /// nested enumeration object
  enum BoundValues { bv_radius = 0, bv_halfZ = 1, bv_length = 2 };

  /// Constructor
  /// @param radius is the radius of the cylinder, default = 0.
  /// @param halez is the half length in z, defualt = 0.
  LineBounds(double radius = 0., double halez = 0.);

  /// Copy constructor
  /// calls teh base copy constructor
  LineBounds(const LineBounds& lbo) : SurfaceBounds(lbo) {}
  /// Destructor
  virtual ~LineBounds();

  /// Assignment operator
  LineBounds&
  operator=(const LineBounds& libo);

  /// Virtual constructor
  virtual LineBounds*
  clone() const override;

  /// Return of the bounds type
  virtual BoundsType
  type() const override
  {
    return SurfaceBounds::Line;
  }

  /// @copydoc SurfaceBounds::inside
  virtual bool
  inside(const Vector2D& lpos, const BoundaryCheck& bchk) const override;

  /// @copydoc Surface::insideLoc0
  virtual bool
  insideLoc0(const Vector2D& lpos, double tol0 = 0.) const override;

  /// @copydoc Surface::insideLoc1
  virtual bool
  insideLoc1(const Vector2D& lpos, double tol1 = 0.) const override;

  /// Minimal distance to boundary
  /// return minimal distance to boundary ( > 0 if outside and <=0 if inside)
  virtual double
  minDistance(const Vector2D& lpos) const override;

  /// This method returns the radius
  virtual double
  r() const;

  /// This method returns the halflengthZ
  double
  halflengthZ() const;

  /// Output Method for std::ostream
  virtual std::ostream&
  dump(std::ostream& sl) const override;

private:
  /// private helper method
  bool
  insideLocR(double r, double tol0) const;

  /// private helper method
  bool
  insideLocZ(double z, double tol1) const;

  /// private method for inside check
  bool
  inside(const Vector2D& lpos, double tol0, double tol1) const;
};

inline LineBounds*
LineBounds::clone() const
{
  return new LineBounds(*this);
}

inline bool
LineBounds::inside(const Vector2D& lpos, const BoundaryCheck& bchk) const
{
  // fast exit - this is a public interface method
  if (!m_valueStore.size()) return true;
  // check with tolerance
  if (bchk.bcType == 0 || bchk.nSigmas == 0)
    return LineBounds::inside(lpos, bchk.toleranceLoc0, bchk.toleranceLoc1);
  // ellipsoid check
  float theta = ((*bchk.lCovariance)(1, 0) != 0
                 && ((*bchk.lCovariance)(1, 1) - (*bchk.lCovariance)(0, 0)) != 0)
      ? .5
          * bchk.FastArcTan(2 * (*bchk.lCovariance)(1, 0)
                            / ((*bchk.lCovariance)(1, 1) - (*bchk.lCovariance)(0, 0)))
      : 0.;
  sincosCache scResult = bchk.FastSinCos(theta);
  double      dphi     = scResult.sinC * scResult.sinC * (*bchk.lCovariance)(0, 0);
  double      dz       = scResult.cosC * scResult.cosC * (*bchk.lCovariance)(0, 1);
  double      max_ell  = dphi > dz ? dphi : dz;
  double      limit    = bchk.nSigmas * sqrt(max_ell);
  return insideLocZ(lpos[Acts::eLOC_Z], limit);
}

inline bool
LineBounds::insideLoc0(const Vector2D& lpos, double tol0) const
{
  // fast exit - this is a public interface method
  if (!m_valueStore.size()) return true;
  return insideLocR(lpos[Acts::eLOC_R], tol0);
}

inline bool
LineBounds::insideLoc1(const Vector2D& lpos, double tol1) const
{
  // fast exit - this is a public interface method
  if (!m_valueStore.size()) return true;
  return insideLocZ(lpos[Acts::eLOC_Z], tol1);
}

inline bool
LineBounds::inside(const Vector2D& lpos, double tol0, double tol1) const
{
  return insideLocR(lpos[Acts::eLOC_R], tol0)
      && insideLocZ(lpos[Acts::eLOC_Z], tol1);
}

inline bool
LineBounds::insideLocR(double r, double tol0) const
{
  return fabs(m_valueStore.at(LineBounds::bv_radius) - r) < tol0;
}

inline bool
LineBounds::insideLocZ(double z, double tol1) const
{
  return (m_valueStore.at(LineBounds::bv_halfZ) + tol1) - fabs(z) > 0.;
}

inline double
LineBounds::r() const
{
  return m_valueStore.at(LineBounds::bv_radius);
}

inline double
LineBounds::halflengthZ() const
{
  return m_valueStore.at(LineBounds::bv_halfZ);
}

}  // end of namespace

#endif  // ACTS_SURFACES_LINEBOUNDS_H