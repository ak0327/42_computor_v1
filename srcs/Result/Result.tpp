#pragma once

# include <stdexcept>

template <typename OkType, typename ErrType>
Result<OkType, ErrType>::Result()
    : is_ok_(false),
      ok_value_(OkType()),
      err_value_(ErrType()) {}


template <typename OkType, typename ErrType>
Result<OkType, ErrType>::Result(const Result &other)
    : is_ok_(other.is_ok_),
      ok_value_(other.ok_value_),
      err_value_(other.err_value_) {}


template <typename OkType, typename ErrType>
Result<OkType, ErrType>::~Result() {}


template <typename OkType, typename ErrType>
Result<OkType, ErrType> &Result<OkType, ErrType>::operator=(const Result &rhs) {
    if (this == &rhs) {
        return *this;
    }
    is_ok_ = rhs.is_ok_;
    ok_value_ = rhs.ok_value_;
    err_value_ = rhs.err_value_;
    return *this;
}


template <typename OkType, typename ErrType>
Result<OkType, ErrType> Result<OkType, ErrType>::ok(const OkType &value) noexcept(true) {
    Result res;
    res.is_ok_ = true;
    res.ok_value_ = value;
    return res;
}


template <typename OkType, typename ErrType>
Result<OkType, ErrType>Result<OkType, ErrType>::err(const ErrType &value) noexcept(true) {
    Result res;
    res.is_ok_ = false;
    res.err_value_ = value;
    return res;
}


template <typename OkType, typename ErrType>
bool Result<OkType, ErrType>::is_ok() const noexcept(true) { return is_ok_; }


template <typename OkType, typename ErrType>
bool Result<OkType, ErrType>::is_err() const noexcept(true) { return !is_ok_; }


template <typename OkType, typename ErrType>
OkType Result<OkType, ErrType>::ok_value() const noexcept(false) {
    if (is_ok_) {
        return ok_value_;
    }
    throw std::runtime_error("[Result Error] Result is not OK");
}


template <typename OkType, typename ErrType>
ErrType Result<OkType, ErrType>::err_value() const noexcept(false) {
    if (!is_ok_) {
        return err_value_;
    }
    throw std::runtime_error("[Result Error] Result is not ERROR");
}
