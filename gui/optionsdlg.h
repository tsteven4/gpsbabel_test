// -*- C++ -*-
// $Id: optionsdlg.h,v 1.2 2009-11-02 20:38:02 robertl Exp $
//------------------------------------------------------------------------
//
//  Copyright (C) 2009  S. Khai Mong <khai@mangrai.com>.
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License as
//  published by the Free Software Foundation; either version 2 of the
//  License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
//  USA.
//
//------------------------------------------------------------------------

#ifndef OPTIONSDLG_H
#define OPTIONSDLG_H

#include <QCheckBox>         // for QCheckBox
#include <QDialog>           // for QDialog
#include <QDialogButtonBox>  // for QDialogButtonBox
#include <QLineEdit>         // for QLineEdit
#include <QList>             // for QList
#include <QObject>           // for QObject, Q_OBJECT, slots
#include <QString>           // for QString
#include <QToolButton>       // for QToolButton
#include <QWidget>           // for QWidget
#include "format.h"          // for FormatOptio

class FileDlgManager: public QObject
{
  Q_OBJECT
public:
  FileDlgManager(QObject* parent,
                 QLineEdit* le,
                 QToolButton* tb, bool isInFile);

private:
  QLineEdit* le;
  QToolButton* tb;
  bool isInFile;

private slots:
  void buttonClicked();

};

class OptionsDlg: public QDialog
{
  Q_OBJECT
public:
  OptionsDlg(QWidget* parent, const QString& fmtName_, QList<FormatOption>* options_,
             const QString& html_);

private:
  QString fmtName_;
  QList<FormatOption>& options_;
  QDialogButtonBox* buttonBox_;
  QList<QCheckBox*> checkBoxes_;
  QList<QWidget*> fields_;
  QString html_;

private slots:
  void acceptClicked();
  void rejectClicked();
  void helpClicked();

};


#endif
