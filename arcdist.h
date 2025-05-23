/*
    Distance from point to arc filter

    Copyright (C) 2002-2014 Robert Lipe, robertlipe+source@gpsbabel.org

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

 */

#ifndef ARCDIST_H_INCLUDED_
#define ARCDIST_H_INCLUDED_

#include <QList>     // for QList
#include <QString>   // for QString
#include <QVector>   // for QVector

#include "defs.h"    // for ARG_NOMINMAX, ARGTYPE_BOOL, Waypoint (ptr only)
#include "filter.h"  // for Filter
#include "option.h"  // for OptionBool, OptionString

#if FILTERS_ENABLED

class ArcDistanceFilter:public Filter
{
public:
  QVector<arglist_t>* get_args() override
  {
    return &args;
  }
  void init() override;
  void process() override;

private:
  /* Types */

  struct extra_data {
    double distance;
    PositionDeg prjpos;
    double frac;
    const Waypoint* arcpt1;
    const Waypoint* arcpt2;
  };

  /* Member Functions */

  void arcdist_arc_disp_wpt_cb(const Waypoint* arcpt2);
  void arcdist_arc_disp_hdr_cb(const route_head* /*unused*/);

  /* Data Members */

  double pos_dist{};
  OptionDouble distopt{true};
  OptionString arcfileopt;
  OptionBool rteopt;
  OptionBool trkopt;
  OptionBool exclopt;
  OptionBool ptsopt;
  OptionBool projectopt;

  QVector<arglist_t> args = {
    {
      "file", &arcfileopt, "File containing vertices of arc",
      nullptr, ARGTYPE_FILE, ARG_NOMINMAX, nullptr
    },
    {
      "rte", &rteopt, "Route(s) are vertices of arc",
      nullptr, ARGTYPE_BOOL, ARG_NOMINMAX, nullptr
    },
    {
      "trk", &trkopt, "Track(s) are vertices of arc",
      nullptr, ARGTYPE_BOOL, ARG_NOMINMAX, nullptr
    },
    {
      "distance", &distopt, "Maximum distance from arc",
      nullptr, ARGTYPE_STRING | ARGTYPE_REQUIRED, ARG_NOMINMAX, nullptr
    },
    {
      "exclude", &exclopt, "Exclude points close to the arc", nullptr,
      ARGTYPE_BOOL, ARG_NOMINMAX, nullptr
    },
    {
      "points", &ptsopt, "Use distance from vertices not lines",
      nullptr, ARGTYPE_BOOL, ARG_NOMINMAX, nullptr
    },
    {
      "project", &projectopt, "Move waypoints to its projection on lines or vertices",
      nullptr, ARGTYPE_BOOL, ARG_NOMINMAX, nullptr
    },
  };

};
#endif // FILTERS_ENABLED
#endif // ARCDIST_H_INCLUDED_
