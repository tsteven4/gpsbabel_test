/*

    Transformation filter for GPS data.

    Copyright (C) 2006 Olaf Klein, o.b.klein@gpsbabel.org

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

#ifndef TRANSFORM_H_INCLUDED_
#define TRANSFORM_H_INCLUDED_

#include <QList>     // for QList
#include <QString>   // for QString
#include <QVector>   // for QVector

#include "defs.h"    // for route_head (ptr only), ARG_NOMINMAX, ARGTY...
#include "filter.h"  // for Filter
#include "option.h"  // for OptionString, OptionBool

#if FILTERS_ENABLED

class TransformFilter:public Filter
{
public:
  QVector<arglist_t>* get_args() override
  {
    return &args;
  }
  void process() override;

private:
  char current_target{};
  route_head* current_trk{};
  route_head* current_rte{};

  OptionString opt_routes;
  OptionString opt_tracks;
  OptionString opt_waypts;
  OptionBool opt_delete;
  OptionInt rpt_name_digits;
  OptionBool opt_rpt_name;
  OptionBool opt_timeless;
  bool timeless{};
  bool use_src_name{};
  QString current_namepart;

  int name_digits{};

  const QString RPT = "RPT";

  QVector<arglist_t> args = {
    {
      "wpt", &opt_waypts, "Transform track(s) or route(s) into waypoint(s) [R/T]", nullptr,
      ARGTYPE_STRING, ARG_NOMINMAX, nullptr
    },
    {
      "rte", &opt_routes, "Transform waypoint(s) or track(s) into route(s) [W/T]", nullptr,
      ARGTYPE_STRING, ARG_NOMINMAX, nullptr
    },
    {
      "trk", &opt_tracks, "Transform waypoint(s) or route(s) into tracks(s) [W/R]", nullptr,
      ARGTYPE_STRING, ARG_NOMINMAX, nullptr
    },
    {
      "rptdigits", &rpt_name_digits, "Number of digits in generated names", nullptr,
      ARGTYPE_INT, "2", nullptr, nullptr
    },
    {
      "rptname", &opt_rpt_name, "Use source name for route point names", "N",
      ARGTYPE_BOOL, ARG_NOMINMAX, nullptr
    },
    {
      "del", &opt_delete, "Delete source data after transformation", "N",
      ARGTYPE_BOOL, ARG_NOMINMAX, nullptr
    },
    {
      "timeless", &opt_timeless, "Create transformed points without times", "N",
      ARGTYPE_BOOL, ARG_NOMINMAX, nullptr
    }
  };

  void transform_waypoints();
  void transform_rte_disp_hdr_cb(const route_head* rte);
  void transform_trk_disp_hdr_cb(const route_head* trk);
  void transform_any_disp_wpt_cb(const Waypoint* wpt);
  void transform_routes();
  void transform_tracks();

};

#endif // FILTERS_ENABLED
#endif // TRANSFORM_H_INCLUDED_
