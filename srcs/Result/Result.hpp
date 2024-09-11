#pragma once

template <typename OkType, typename ErrType>
class Result {
 public:
	Result();
	Result(const Result &other);
	~Result();

	Result &operator=(const Result &rhs);

	static Result ok(const OkType &value) noexcept(true);
	static Result err(const ErrType &value) noexcept(true);

	bool is_ok() const noexcept(true);
	bool is_err() const noexcept(true);

	OkType ok_value() const noexcept(false);

	ErrType err_value() const noexcept(false);
 private:
	bool is_ok_;
	OkType ok_value_;
	ErrType err_value_;
};

#include "Result.tpp"
