/*
    Library for inifile like data files.

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

#include "defs.h"              // for gbFatal, gbWarning
#include "inifile.h"
#include "src/core/file.h"     // for File
#include <QByteArray>          // for QByteArray
#include <QChar>               // for operator==, QChar
#include <QDir>                // for QDir
#include <QFile>               // for QFile
#include <QFileInfo>           // for QFileInfo
#include <QHash>               // for QHash
#include <QIODevice>           // for QIODevice::ReadOnly, QIODevice
#include <QTextStream>         // for QTextStream
#include <QtGlobal>            // for qEnvironmentVariable, qPrintable
#include <utility>


struct inifile_t {
  QHash<QString, InifileSection> sections;
  QString source;
};

class InifileSection
{
public:
  QString name;
  QHash<QString, QString> entries;

  InifileSection() = default;
  explicit InifileSection(QString nm) : name{std::move(nm)} {}
};

/* internal procedures */

static constexpr char GPSBABEL_INIFILE[] = "gpsbabel.ini";
#ifndef __WIN32__
static constexpr char GPSBABEL_SUBDIR[] = ".gpsbabel";
#endif


static QString
find_gpsbabel_inifile(const QString& path)  /* can be empty or NULL */
{
  if (path.isNull()) {
    return QString();
  }
  QString inipath(QDir(path).filePath(GPSBABEL_INIFILE));
  return QFile(inipath).open(QIODevice::ReadOnly) ? inipath : QString();
}

static QString
open_gpsbabel_inifile()
{
  QString res;

  QString envstr = qEnvironmentVariable("GPSBABELINI");
  if (!envstr.isNull()) {
    if (QFile(envstr).open(QIODevice::ReadOnly)) {
      return envstr;
    }
    gbWarning("WARNING: GPSBabel-inifile, defined in environment, NOT found!\n");
    return res;
  }
  QString name = find_gpsbabel_inifile("");  // Check in current directory first.
  if (name.isNull()) {
#ifdef __WIN32__
    // Use &&'s early-out behaviour to try successive file locations: first
    // %APPDATA%, then %WINDIR%, then %SYSTEMROOT%.
    (name = find_gpsbabel_inifile(qEnvironmentVariable("APPDATA"))).isNull()
    && (name = find_gpsbabel_inifile(qEnvironmentVariable("WINDIR"))).isNull()
    && (name = find_gpsbabel_inifile(qEnvironmentVariable("SYSTEMROOT"))).isNull();
#else
    // Use &&'s early-out behaviour to try successive file locations: first
    // ~/.gpsbabel, then /usr/local/etc, then /etc.
    (name = find_gpsbabel_inifile(QDir::home().filePath(GPSBABEL_SUBDIR))).
    isNull()
    && (name = find_gpsbabel_inifile("/usr/local/etc")).isNull()
    && (name = find_gpsbabel_inifile("/etc")).isNull();
#endif
  }
  if (!name.isNull()) {
    res = name;
  }
  return res;
}

static void
inifile_load_file(QTextStream* stream, inifile_t* inifile)
{
  QString buf;
  InifileSection section;

  while (!(buf = stream->readLine()).isNull()) {
    buf = buf.trimmed();

    if (buf.isEmpty()) {
      continue;  /* skip empty lines */
    }
    if ((buf.at(0) == '#') || (buf.at(0) == ';')) {
      continue;  /* skip comments */
    }

    if (buf.at(0) == '[') {
      QString section_name;
      if (buf.contains(']')) {
        section_name = buf.mid(1, buf.indexOf(']') - 1).trimmed();
      }
      if (section_name.isEmpty()) {
        gbFatal("invalid section header '%s' in '%s'.\n", gbLogCStr(section_name),
              gbLogCStr(inifile->source));
      }

      // form lowercase key to implement CaseInsensitive matching.
      section_name = section_name.toLower();
      inifile->sections.insert(section_name, InifileSection(section_name));
      section = inifile->sections.value(section_name);
    } else {
      if (section.name.isEmpty()) {
        gbFatal("missing section header in '%s'.\n",
              gbLogCStr(inifile->source));
      }

      // Store key in lower case to implement CaseInsensitive matching.
      QString key = buf.section('=', 0, 0).trimmed().toLower();
      // Take some care so the return from inifile_find_value can
      // be used to distinguish between a key that isn't found
      // and a found key without a value, i.e. force value
      // to be non-null but possibly empty.
      QString value = buf.section('=', 1).append("").trimmed();
      section.entries.insert(key, value);

      // update the QHash sections with the modified InifileSection section.
      inifile->sections.insert(section.name, section);
    }
  }
}

static QString
inifile_find_value(const inifile_t* inifile, const QString& sec_name, const QString& key)
{
  if (inifile == nullptr) {
    return QString();
  }

  // CaseInsensitive matching implemented by forcing sec_name & key to lower case.
  return inifile->sections.value(sec_name.toLower()).entries.value(key.toLower());
}

/* public procedures */

/*
	inifile_init:
	  reads inifile filename into memory

	  filename == NULL: try to open global gpsbabel.ini
 */
inifile_t*
inifile_init(const QString& filename)
{
  QString name;

  if (filename.isEmpty()) {
    name = open_gpsbabel_inifile();
    if (name.isEmpty()) {
      return nullptr;
    }
  } else {
    name = filename;
  }

  gpsbabel::File file(name);
  file.open(QFile::ReadOnly);
  QTextStream stream(&file);
  // default for QTextStream::setEncoding in Qt6 is QStringConverter::Utf8
  stream.setAutoDetectUnicode(true);

  auto* result = new inifile_t;
  QFileInfo fileinfo(file);
  result->source = fileinfo.absoluteFilePath();
  inifile_load_file(&stream, result);

  file.close();
  return result;
}

void
inifile_done(inifile_t* inifile)
{
  delete inifile;
}

bool
inifile_has_section(const inifile_t* inifile, const QString& section)
{
  return inifile->sections.contains(section.toLower());
}

/*
     inifile_readstr:
       returns a null QString if not found, otherwise a non-null but possibly
       empty Qstring with the value of key ...
 */

QString
inifile_readstr(const inifile_t* inifile, const QString& section, const QString& key)
{
  return inifile_find_value(inifile, section, key);
}

/*
     inifile_readint:
       on success the value is stored into "*value" and "inifile_readint" returns 1,
       otherwise inifile_readint returns 0
 */

int
inifile_readint(const inifile_t* inifile, const QString& section, const QString& key, int* value)
{
  const QString str = inifile_find_value(inifile, section, key);

  if (str.isNull()) {
    return 0;
  }

  if (value != nullptr) {
    *value = str.toInt();
  }
  return 1;
}

/*
     inifile_readint_def:
       if found inifile_readint_def returns value of key, otherwise a default value "def"
 */

int
inifile_readint_def(const inifile_t* inifile, const QString& section, const QString& key, const int def)
{
  int result;

  if (inifile_readint(inifile, section, key, &result) == 0) {
    return def;
  } else {
    return result;
  }
}

