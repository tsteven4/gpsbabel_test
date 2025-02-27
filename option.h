/*
    Copyright (C) 2024 Robert Lipe, robertlipe+source@gpsbabel.org

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
#ifndef OPTION_H_INCLUDED_
#define OPTION_H_INCLUDED_

#include <QString>     // for QString, operator!=

class Option /* Abstract Class */
{
public:
  /* Special Member Functions */
  Option() = default;
  // Provide virtual public destructor to avoid undefined behavior when
  // an object of derived class type is deleted through a pointer to
  // its base class type.
  // https://wiki.sei.cmu.edu/confluence/display/cplusplus/OOP52-CPP.+Do+not+delete+a+polymorphic+object+without+a+virtual+destructor
  virtual ~Option() = default;
  // And that requires us to explicitly default or delete the move and copy operations.
  // To prevent slicing we delete them.
  // https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c21-if-you-define-or-delete-any-default-operation-define-or-delete-them-all.
  Option(const Option&) = delete;
  Option& operator=(const Option&) = delete;
  Option(Option&&) = delete;
  Option& operator=(Option&&) = delete;

  /* Member Functions */
  [[nodiscard]] virtual bool has_value() const = 0;
  [[nodiscard]] virtual bool isEmpty() const = 0;
  [[nodiscard]] virtual const QString& get() const = 0;
  virtual void init(const QString& id) {}
  virtual void reset() = 0;
  virtual void set(const QString& s) = 0;

  /* Data Members */
  // I.25: Prefer empty abstract classes as interfaces to class hierarchies
  // https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#i25-prefer-empty-abstract-classes-as-interfaces-to-class-hierarchies
};

class OptionString : public Option
{
public:
  /* Special Member Functions */
  OptionString() = default;

  explicit(false) operator const QString& () const
  {
    return value_;
  }

  explicit(false) operator bool () const
  {
    return !value_.isNull();
  }

  [[nodiscard]] bool has_value() const override
  {
    return !value_.isNull();
  }

  [[nodiscard]] bool isEmpty() const override
  {
    return value_.isEmpty();
  }

  [[nodiscard]] const QString& get() const override
  {
    return value_;
  }

  void init(const QString& id) override
  {
    id_ = id;
  }

  void reset() override
  {
    value_ = QString();
  }

  void set(const QString& s) override
  {
    value_ = s;
  }

// We use overloads instead of default parameters to enable tool visibility into different usages.
  int toInt() const;
  int toInt(bool* ok) const;
  int toInt(bool* ok, QString* end, int base) const;
  double toDouble() const;
  double toDouble(bool* ok) const;
  double toDouble(bool* ok, QString* end) const;

private:
  QString value_;
  QString id_;
};

class OptionInt : public Option
{
public:
  /* Special Member Functions */
  OptionInt() = default;
  explicit OptionInt(bool allow_trailing_data, int base) :
    allow_trailing_data_(allow_trailing_data),
    base_(base)
  {}

  explicit(false) operator const QString& () const
  {
    return value_;
  }

  explicit(false) operator bool () const
  {
    return !value_.isNull();
  }

  [[nodiscard]] bool has_value() const override
  {
    return !value_.isNull();
  }

  [[nodiscard]] bool isEmpty() const override
  {
    return value_.isEmpty();
  }

  [[nodiscard]] const QString& get() const override
  {
    return value_;
  }

  void init(const QString& id) override;
  void reset() override;
  void set(const QString& s) override;
  bool isValid(const QString& s) const;
  int get_result(QString* end = nullptr) const;
  bool trailing_data_allowed() const;

private:
  QString value_;
  QString id_;
  int result_{};
  QString end_;
  bool allow_trailing_data_{false};
  int base_{10};
};

class OptionDouble : public Option
{
public:
  /* Special Member Functions */
  OptionDouble() = default;
  explicit OptionDouble(bool allow_trailing_data) :
    allow_trailing_data_(allow_trailing_data)
  {}

  explicit(false) operator const QString& () const
  {
    return value_;
  }

  explicit(false) operator bool () const
  {
    return !value_.isNull();
  }

  [[nodiscard]] bool has_value() const override
  {
    return !value_.isNull();
  }

  [[nodiscard]] bool isEmpty() const override
  {
    return value_.isEmpty();
  }

  [[nodiscard]] const QString& get() const override
  {
    return value_;
  }

  void init(const QString& id) override;
  void reset() override;
  void set(const QString& s) override;
  bool isValid(const QString& s) const;
  double get_result(QString* end = nullptr) const;
  bool trailing_data_allowed() const;

private:
  QString value_;
  QString id_;
  double result_{};
  QString end_;
  bool allow_trailing_data_{false};
};

class OptionBool : public Option
{
public:
  /* Special Member Functions */
  OptionBool() = default;

  /* Traditionally un-supplied bool options without default are considered to be false. */
  explicit(false) operator bool() const
  {
    return (!value_.isNull() && (value_ != '0'));
  }

  /* Note that has_value can be used to distinguish an option that wasn't supplied
   * from one that was supplied and is considered false by Vecs::assign_option.
   */
  [[nodiscard]] bool has_value() const override
  {
    return !value_.isNull();
  }

  [[nodiscard]] bool isEmpty() const override
  {
    return value_.isEmpty();
  }

  [[nodiscard]] const QString& get() const override
  {
    return value_;
  }

  void reset() override
  {
    value_ = QString();
  }

  void set(const QString& s) override
  {
    value_ = s;
  }

private:
  QString value_;
};
#endif // OPTION_H_INCLUDED_
