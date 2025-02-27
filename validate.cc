/*

    validate internal data structures.

    Copyright (C) 2013 Robert Lipe   robertlipe+source@gpsbabel.org

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

#include "defs.h"
#include "validate.h"

#if FILTERS_ENABLED

void ValidateFilter::validate_head(const route_head* /*unused*/)
{
  head_ct += 1;
  point_ct = 0;
  segment_ct = 0;
}

void ValidateFilter::validate_head_trl(const route_head* header)
{
  total_point_ct += point_ct;
  total_segment_ct += segment_ct;
  if (opt_debug) {
    gbDebug("%s %d ct: %d, waypt_count: %d, segments %d\n", segment_type, header->rte_num,  point_ct, header->rte_waypt_ct(), segment_ct);
  }
  if (!opt_debug && (point_ct != header->rte_waypt_ct())) {
    gbFatal("%s %d count mismatch, expected %d, actual %d\n", segment_type, header->rte_num, header->rte_waypt_ct(), point_ct);
  }
}

void ValidateFilter::validate_point(const Waypoint* wpt)
{
  point_ct += 1;
  if (wpt->wpt_flags.new_trkseg) {
    segment_ct += 1;
  }
}

void ValidateFilter::process()
{
  WayptFunctor<ValidateFilter> validate_point_f(this, &ValidateFilter::validate_point);
  RteHdFunctor<ValidateFilter> validate_head_f(this, &ValidateFilter::validate_head);
  RteHdFunctor<ValidateFilter> validate_head_trl_f(this, &ValidateFilter::validate_head_trl);

  point_ct = 0;
  if (opt_debug) {
    gbDebug("\nProcessing waypts\n");
  }
  waypt_disp_all(validate_point_f);
  if (opt_debug) {
    gbDebug("point ct: %d, waypt_count: %d\n", point_ct, waypt_count());
  }
  if (!opt_debug && (point_ct != waypt_count())) {
    gbFatal("Waypoint count mismatch, expected %d, actual %d\n", waypt_count(), point_ct);
  }

  head_ct = 0;
  total_point_ct = 0;
  total_segment_ct = 0;
  segment_type = "route";
  if (opt_debug) {
    gbDebug("\nProcessing routes\n");
  }
  route_disp_all(validate_head_f, validate_head_trl_f, validate_point_f);
  if (opt_debug) {
    gbDebug("route head ct: %d, route_count: %d, total segment count: %d\n", head_ct, route_count(), total_segment_ct);
    gbDebug("total route point ct: %d, route_waypt_count: %d\n", total_point_ct, route_waypt_count());
  }
  if (!opt_debug && (head_ct != route_count())) {
    gbFatal("Route count mismatch, expected %d, actual %d\n", route_count(), head_ct);
  }
  if (!opt_debug && (total_point_ct != route_waypt_count())) {
    gbFatal("Total route waypoint count mismatch, expected %d, actual %d\n", route_waypt_count(), total_point_ct);
  }

  head_ct = 0;
  total_point_ct = 0;
  total_segment_ct = 0;
  segment_type = "track";
  if (opt_debug) {
    gbDebug("\nProcessing tracks\n");
  }
  track_disp_all(validate_head_f, validate_head_trl_f, validate_point_f);
  if (opt_debug) {
    gbDebug("track head ct: %d, track_count: %d, total segment count: %d\n", head_ct, track_count(), total_segment_ct);
    gbDebug("total track point ct: %d, track_waypt_count: %d\n", total_point_ct, track_waypt_count());
  }
  if (!opt_debug && (head_ct != track_count())) {
    gbFatal("Track count mismatch, expected %d, actual %d\n", track_count(), head_ct);
  }
  if (!opt_debug && (total_point_ct != track_waypt_count())) {
    gbFatal("Total track waypoint count mismatch, expected %d, actual %d\n", track_waypt_count(), total_point_ct);
  }

  if (opt_checkempty) {
    if (waypt_count()==0 && route_waypt_count()==0 && track_waypt_count()==0) {
      gbFatal("No input\n");
    }
  }
}

#endif
