if(APPLE)
  cmake_minimum_required(VERSION 3.21.1)
else()
  cmake_minimum_required(VERSION 3.16)
endif()

include(CMakeDependentOption)
include(CheckIncludeFile)

include(gbversion.cmake)
project(gpsbabel LANGUAGES C CXX VERSION ${GB.VERSION})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Do this after we set up common variables but before creating other
# variables that will be inherited.
add_subdirectory(gui)

# FIXME: When we rearrange the project directory structure everything
# below here should be in it's own CMakeList.txt

configure_file(gbversion.h.in gbversion.h @ONLY NEWLINE_STYLE LF)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Handle the Qt rcc code generator automatically
set(CMAKE_AUTORCC ON)

add_executable(gpsbabel)

# Find the QtCore library
find_package(Qt6 REQUIRED COMPONENTS Core Core5Compat)
list(APPEND QT_LIBRARIES Qt6::Core Qt6::Core5Compat)
if(${Qt6Core_VERSION} VERSION_LESS 6.2)
  message(FATAL_ERROR "Qt version ${Qt6Core_VERSION} found, but version 6.2 or newer is required.")
else()
  message(STATUS "Using Qt6 version ${Qt6Core_VERSION}")
endif()
if(${Qt6Core_VERSION} VERSION_GREATER_EQUAL 6.5)
  target_compile_definitions(gpsbabel PRIVATE LIGHTWEIGHT_TIMEZONES_SUPPORTED)
endif()

option(GPSBABEL_ENABLE_PCH "enable precompiled headers." ON)
if (GPSBABEL_ENABLE_PCH)
  # This list is from defs.h.
  target_precompile_headers(gpsbabel PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:<cmath$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<cstddef$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<cstdint$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<cstdio$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<ctime$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<numbers$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<optional$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<utility$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<QByteArray$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<QDate$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<QTime$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<QDateTime$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<QDebug$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<QList$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<QString$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<QStringView$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<QTextCodec$<ANGLE-R>>"
  )
  if(${Qt6Core_VERSION} VERSION_GREATER_EQUAL 6.5)
    target_precompile_headers(gpsbabel PRIVATE
      "$<$<COMPILE_LANGUAGE:CXX>:<QTimeZone$<ANGLE-R>>"
    )
  endif()
  target_precompile_headers(gpsbabel PRIVATE
    "$<$<COMPILE_LANGUAGE:CXX>:<Qt$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<QtGlobal$<ANGLE-R>>"
  )
endif()

# RESOURCES
set(RESOURCES gpsbabel.qrc)

# MINIMAL_FMTS
set(MINIMAL_FMTS
  garmin.cc
  garmin_tables.cc
  geo.cc
  gpx.cc
  kml.cc
  nmea.cc
)

# ALL_FMTS
set(ALL_FMTS ${MINIMAL_FMTS}
  dg-100.cc
  exif.cc
  garmin_fit.cc
  garmin_gpi.cc
  garmin_txt.cc
  garmin_xt.cc
  gdb.cc
  geocache.cc
  geojson.cc
  globalsat_sport.cc
  googletakeout.cc
  gtm.cc
  gtrnctr.cc
  html.cc
  humminbird.cc
  igc.cc
  lowranceusr.cc
  mtk_logger.cc
  osm.cc
  ozi.cc
  qstarz_bl_1000.cc
  random.cc
  seventymai.cc
  shape.cc
  skytraq.cc
  subrip.cc
  text.cc
  tpg.cc
  tpo.cc
  unicsv.cc
  v900.cc
  vcf.cc
  xcsv.cc
)

# ALL_FMTS = $$MINIMAL_FMTS

# FILTERS
set(FILTERS
  arcdist.cc
  bend.cc
  discard.cc
  duplicate.cc
  height.cc
  interpolate.cc
  nukedata.cc
  polygon.cc
  position.cc
  radius.cc
  resample.cc
  reverse_route.cc
  smplrout.cc
  sort.cc
  stackfilter.cc
  swapdata.cc
  trackfilter.cc
  transform.cc
  validate.cc
)

# JEEPS
set(JEEPS
  jeeps/gpsapp.cc
  jeeps/gpscom.cc
  jeeps/gpsdevice.cc
  jeeps/gpsdevice_ser.cc
  jeeps/gpsdevice_usb.cc
  jeeps/gpsmath.cc
  jeeps/gpsmem.cc
  jeeps/gpsprot.cc
  jeeps/gpsread.cc
  jeeps/gpsrqst.cc
  jeeps/gpssend.cc
  jeeps/gpsserial.cc
  jeeps/gpsusbcommon.cc
  jeeps/gpsusbread.cc
  jeeps/gpsusbsend.cc
  jeeps/gpsutil.cc
)

# SUPPORT
set(SUPPORT
  csv_util.cc
  fatal.cc
  filter_vecs.cc
  formspec.cc
  garmin_fs.cc
  gbfile.cc
  gbser.cc
  globals.cc
  grtcirc.cc
  inifile.cc
  main.cc
  mkshort.cc
  option.cc
  parse.cc
  rgbcolors.cc
  route.cc
  session.cc
  src/core/codecdevice.cc
  src/core/logging.cc
  src/core/nvector.cc
  src/core/textstream.cc
  src/core/usasciicodec.cc
  src/core/vector3d.cc
  src/core/xmlstreamwriter.cc
  src/core/xmltag.cc
  units.cc
  util.cc
  vecs.cc
  waypt.cc
  xmlgeneric.cc
)

# HEADERS
set(HEADERS
  csv_util.h
  defs.h
  dg-100.h
  exif.h
  filter.h
  filter_vecs.h
  format.h
  formspec.h
  garmin.h
  garmin_fit.h
  garmin_fs.h
  garmin_gpi.h
  garmin_icon_tables.h
  garmin_tables.h
  gbfile.h
  gbser.h
  gbser_private.h
  garmin_txt.h
  garmin_xt.h
  gdb.h
  geocache.h
  geojson.h
  globalsat_sport.h
  geo.h
  googletakeout.h
  gpx.h
  grtcirc.h
  gtm.h
  gtrnctr.h
  heightgrid.h
  humminbird.h
  html.h
  inifile.h
  kml.h
  igc.h
  lowranceusr.h
  mkshort.h
  mtk_logger.h
  nmea.h
  osm.h
  ozi.h
  qstarz_bl_1000.h
  random.h
  session.h
  seventymai.h
  shape.h
  skytraq.h
  subrip.h
  text.h
  tpg.h
  tpo.h
  unicsv.h
  units.h
  v900.h
  vcf.h
  vecs.h
  xcsv.h
  xmlgeneric.h
  jeeps/garminusb.h
  jeeps/gps.h
  jeeps/gpsapp.h
  jeeps/gpscom.h
  jeeps/gpsdatum.h
  jeeps/gpsdevice.h
  jeeps/gpsmath.h
  jeeps/gpsmem.h
  jeeps/gpsport.h
  jeeps/gpsprot.h
  jeeps/gpsread.h
  jeeps/gpsrqst.h
  jeeps/gpssend.h
  jeeps/gpsserial.h
  jeeps/gpsusbcommon.h
  jeeps/gpsusbint.h
  jeeps/gpsutil.h
  src/core/codecdevice.h
  src/core/datetime.h
  src/core/file.h
  src/core/logging.h
  src/core/nvector.h
  src/core/textstream.h
  src/core/usasciicodec.h
  src/core/vector3d.h
  src/core/xmlstreamwriter.h
  src/core/xmltag.h
)

string(REPLACE .cc .h FILTER_HEADERS "${FILTERS}")
set(HEADERS ${HEADERS} ${FILTER_HEADERS})

if(UNIX)
  set(SOURCES ${SOURCES} gbser_posix.cc)
  set(HEADERS ${HEADERS} gbser_posix.h)
  target_compile_options(gpsbabel PRIVATE -Wall)
endif()

if(WIN32)
  target_compile_definitions(gpsbabel PRIVATE __WIN32__)
  qt_disable_unicode_defines(gpsbabel)
  if(CMAKE_BUILD_TYPE STREQUAL Debug)
    target_compile_definitions(gpsbabel PRIVATE _DEBUG)
  endif()
  set(SOURCES ${SOURCES} gbser_win.cc)
  set(HEADERS ${HEADERS} gbser_win.h)
  set(JEEPS ${JEEPS} jeeps/gpsusbwin.cc)
  set(LIBS ${LIBS} setupapi)
  set(RESOURCES ${RESOURCES} win32/gpsbabel.rc)
endif()

if(MSVC)
  target_compile_definitions(gpsbabel PRIVATE _CRT_SECURE_NO_WARNINGS)
  target_compile_definitions(gpsbabel PRIVATE _CRT_NONSTDC_NO_WARNINGS)
  target_compile_options(gpsbabel PRIVATE /MP -wd4100 -wd4267 -wd5030)
endif()

if(APPLE)
  target_compile_options(gpsbabel PRIVATE -Wall -Wsign-compare)
endif()

include(shapelib.cmake)
include(zlib.cmake)
include(libusb.cmake)
include(strptime.cmake)

set(GPSBABEL_EXTRA_LINK_LIBRARIES "" CACHE STRING "extra libraries to link with.")
list(APPEND LIBS ${GPSBABEL_EXTRA_LINK_LIBRARIES})
set(GPSBABEL_EXTRA_INCLUDE_DIRECTORIES "" CACHE STRING "extra directories to include.")
target_include_directories(gpsbabel PRIVATE ${GPSBABEL_EXTRA_INCLUDE_DIRECTORIES})
set(GPSBABEL_EXTRA_COMPILE_OPTIONS "" CACHE STRING "extra compile options.")
# Qt sanitize.conf used:
#   -fsanitize=address -fno-omit-frame-pointer
#   -fsanitize=undefined -fsanitize=float-divide-by-zero -fno-omit-frame-pointer
# For coverage use --coverage
separate_arguments(GPSBABEL_EXTRA_COMPILE_OPTIONS)
target_compile_options(gpsbabel PRIVATE ${GPSBABEL_EXTRA_COMPILE_OPTIONS})
set(GPSBABEL_EXTRA_LINK_OPTIONS "" CACHE STRING "extra link options.")
# Qt sanitize.conf used:
#   -fsanitize=address
#   -fsanitize=undefined -fsanitize=float-divide-by-zero
# For coverage use --coverage
separate_arguments(GPSBABEL_EXTRA_LINK_OPTIONS)
target_link_options(gpsbabel PRIVATE ${GPSBABEL_EXTRA_LINK_OPTIONS})

set(SOURCES
  ${SOURCES} ${ALL_FMTS} ${FILTERS} ${SUPPORT} ${SHAPE} ${ZLIB} ${JEEPS} ${RESOURCES}
)

list(SORT SOURCES)
list(SORT HEADERS)

target_sources(gpsbabel PRIVATE ${SOURCES} ${HEADERS})

# We don't care about stripping things out of the build.  Full monty, baby.
target_compile_definitions(gpsbabel PRIVATE FILTERS_ENABLED)

target_link_libraries(gpsbabel PRIVATE ${QT_LIBRARIES} ${LIBS})

get_target_property(Srcs gpsbabel SOURCES)
message(STATUS "Sources are: \"${Srcs}\"")
get_target_property(DirDefs gpsbabel COMPILE_DEFINITIONS)
message(STATUS "Defines are: \"${DirDefs}\"")
get_target_property(LnkLibs gpsbabel LINK_LIBRARIES)
message(STATUS "Libs are: \"${LnkLibs}\"")
get_target_property(IncDirs gpsbabel INCLUDE_DIRECTORIES)
message(STATUS "Include Directores are: \"${IncDirs}\"")

# In case we ever update the garmin_icons mkicondoc helps 
# update the related document table.
add_executable(mkicondoc EXCLUDE_FROM_ALL mkicondoc.cc)
target_link_libraries(mkicondoc PRIVATE ${QT_LIBRARIES})

set(TESTS
  arc-project
  arc
  batch
  bend
  classic-2
  classic-3
  dg100
  dop_filter
  duplicate
  exif
  garmin_fit
  garmin_g1000
  garmin_gpi
  garmin_txt
  garmin_xt
  gbfile
  gdb
  geojson
  geo
  globalsat_sport
  googletakeout
  gpsdrive
  gpx
  grapheme
  gtm
  gtrnctr
  height
  humminbird
  iblue747
  igc
  interpolate
  kml-read
  kml
  lowranceusr
  mkshort
  mtk
  multiurlgpx
  nmea
  osm
  ozi
  polygon
  position
  qstarz_bl_1000
  radius
  realtime
  resample
  route_reverse
  serialization
  seventymai
  shape
  simplify-relative
  simplify
  skytraq
  sort
  stackfilter
  subrip
  swap
  text
  tpg
  tpo
  track-discard
  track
  transform
  unicsv_grids
  unicsv_local
  unicsv
  unitconversion
  v900
  validate_formats
  validate
  vcard
  vecs
  xcsv
)

list(SORT TESTS)

if(UNIX)
  # This test only works if the pwd is top level source dir due to the
  # file name getting embedded in the file nonexistent.err.
  enable_testing()
  foreach(TESTNAME IN LISTS TESTS)
    add_test(NAME test-${TESTNAME}
             COMMAND ${CMAKE_SOURCE_DIR}/testo -p $<TARGET_FILE:gpsbabel> ${TESTNAME}
             WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            )
  endforeach()
endif()

if(UNIX)
  # This test only works if the pwd is top level source dir due to the
  # file name getting embedded in the file nonexistent.err.
  add_custom_target(check
                    ${CMAKE_SOURCE_DIR}/testo -p $<TARGET_FILE:gpsbabel>
                    DEPENDS gpsbabel
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                    VERBATIM
                    USES_TERMINAL)
endif()
if(UNIX AND NOT APPLE)
  # This test only works if the pwd is top level source dir due to the
  # file name getting embedded in the file nonexistent.err.
  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/testo.d)
  list(JOIN TESTS "\\n" VTESTS)
  add_custom_target(check-vtesto
                    printf "${VTESTS}" |
                    xargs -P 3 -I TESTNAME ${CMAKE_SOURCE_DIR}/vtesto
                    -l -j ${CMAKE_BINARY_DIR}/testo.d/TESTNAME.vglog -p $<TARGET_FILE:gpsbabel> TESTNAME
                    DEPENDS gpsbabel
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                    VERBATIM
                    USES_TERMINAL)
endif()

get_property(_isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if((CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR) AND NOT _isMultiConfig)
  set(GPSBABEL_WEB "gpsbabel.org" CACHE PATH "Path where the documentation will be stored for www.gpsbabel.org.")
  add_custom_target(gpsbabel.org
                    ${CMAKE_SOURCE_DIR}/tools/make_gpsbabel_org.sh ${GPSBABEL_WEB} ${GPSBABEL_DOCVERSION}
                    DEPENDS gpsbabel gpsbabel.pdf
                    VERBATIM
                    USES_TERMINAL)

  add_custom_target(gpsbabel.html
                    ${CMAKE_SOURCE_DIR}/tools/make_gpsbabel_html.sh
                    DEPENDS gpsbabel
                    VERBATIM
                    USES_TERMINAL)

  add_custom_target(gpsbabel.pdf
                    ${CMAKE_SOURCE_DIR}/tools/make_gpsbabel_pdf.sh
                    DEPENDS gpsbabel
                    VERBATIM
                    USES_TERMINAL)
else()
  message(WARNING "Document generation is only supported for in-source builds with single configuration generators.")
endif()
