#ifndef MATRIX_CUH_
#define MATRIX_CUH_

#define ZHP_CUDA

// CUDA headers
#include <cuda/error.cuh>
#include <cuda/tensor.cuh>

// Engine headers
#include <matrix.hpp>

#define inline_init_mat(mat, rs, cs)	\
	Matrix <T> other;		\
					\
	other.__rows = rs;		\
	other.__cols = cs;		\
					\
	other.__array = new T[rs * cs];	\
					\
	other.__dims = 2;		\
					\
	other.__dim = new size_t[2];	\
					\
	other.__dim[0] = rs;		\
	other.__dim[1] = cs;

namespace zhetapi {

template <class T>
__host__ __device__
Matrix <T> ::Matrix() : __rows(0), __cols(0), Tensor <T> () {}

template <class T>
__host__ __device__
Matrix <T> ::Matrix(bool dev) : __rows(0), __cols(0), Tensor <T> (dev) {}

template <class T>
__host__ __device__
Matrix <T> ::Matrix(const Matrix <T> &other) : __rows(other.__rows), __cols(other.__cols), Tensor <T>
					       (other.__rows, other.__cols, T())
{
	for (int i = 0; i < __rows; i++) {
		for (int j = 0; j < __cols; j++)
			this->__array[__cols * i + j] = other.__array[__cols * i + j];
	}
}

template <class T>
__host__ __device__
Matrix <T> ::Matrix(const Vector <T> &other) : __rows(other.__rows), __cols(1), Tensor <T>
					       (other.__rows, 1, T())
{
	for (int i = 0; i < __rows; i++)
		this->__array[i] = other.__array[i];
}

// Unified is true iff the matrix is to be allocated onto unified memroy
template <class T>
__host__ __device__
Matrix <T> ::Matrix(size_t rs, size_t cs, T val) : Tensor <T> (rs, cs, T())
{
	__rows = rs;
	__cols = cs;
	
	for (int i = 0; i < __rows; i++) {
		for (int j = 0; j < __cols; j++)
			this->__array[__cols * i + j] = val;
	}
}

template <class T>
template <class F>
__host__ __device__
Matrix <T> ::Matrix(size_t rs, size_t cs, F gen) : Tensor <T> (rs, cs, T())
{
	__rows = rs;
	__cols = cs;
	
	for (int i = 0; i < __rows; i++) {
		for (int j = 0; j < __cols; j++)
			this->__array[__cols * i + j] = gen(i, j);
	}
}

template <class T>
__host__ __device__
const Matrix <T> &Matrix <T> ::operator=(const Matrix <T> &other)
{
	if (this != &other) {
		this->clear();

		this->__array = new T[other.__size];

		this->__rows = other.__rows;
		this->__cols = other.__cols;

		this->__size = other.__size;
		for (size_t i = 0; i < this->__size; i++)
			this->__array[i] = other.__array[i];
		
		this->__dims = 2;
		this->__dim = new size_t[2];

		this->__dim[0] = this->__rows;
		this->__dim[1] = this->__cols;
	}

	return *this;
}

// TODO: Turn this method into a virtual method with the base as Tensor
template <class T>
__host__ __device__
void Matrix <T> ::stable_transfer(const Matrix <T> &other)
{
	// TODO: Add error handling for when the dimensions mismatch
	for (int i = 0; i < this->__size; i++) {
		// printf("Copying %f\n", other.__array[i]);
		this->__array[i] = other.__array[i];
	}
}

template <class T>
__host__ __device__
void Matrix <T> ::stable_shur(const Matrix <T> &other)
{

#ifndef __CUDA_ARCH__

	// TODO: Replace with better exception handling for the device
	if (!((other.get_rows() == __rows)
		&& (other.get_cols() == __cols)))
		throw typename Matrix <T> ::dimension_mismatch();

#endif

	// Fill out the matrix
	for (size_t i = 0; i < this->__size; i++)
		this->__array[i] *= other.__array[i];
}

template <class T>
void Matrix <T> ::allocate_managed(size_t rs, size_t cs, T def)
{
	this->__on_device = true;

	__rows = rs;
	__cols = cs;

	this->__size = rs * cs;
	this->__dims = 2;

	cudaMallocManaged(&this->__dim, 2 * sizeof(size_t));
	cudaCheckError(nullptr);

	this->__dim[0] = rs;
	this->__dim[1] = cs;

	cudaMallocManaged(&this->__array, sizeof(T) * this->__size);
	cudaCheckError(nullptr);

	for (size_t i = 0; i < this->__size; i++)
		this->__array[i] = def;
}

template <class T>
__host__ __device__
void Matrix <T> ::show(int tid) const
{
	printf("{");

	for (size_t i = 0; i < this->__size; i++) {
		printf("%f", this->__array[i]);

		if (i < this->__size - 1)
			printf(", ");
	}

	printf("}\n");
}

template <class T>
__host__ __device__
T *Matrix <T> ::operator[](size_t i)
{
	return (this->__array + i * __cols);
}

template <class T>
__host__ __device__
const T *Matrix <T> ::operator[](size_t i) const
{
	return (this->__array + i * __cols);
}

template <class T>
__host__ __device__
void Matrix <T> ::set_all(T x)
{
	for (size_t i = 0; i < this->__size; i++)
		this->__array[i] = x;
}

template <class T>
__host__ __device__
size_t Matrix <T> ::get_rows() const
{
	return __rows;
}

template <class T>
__host__ __device__
size_t Matrix <T> ::get_cols() const
{
	return __cols;
}

template <class T>
__host__ __device__
Matrix <T> Matrix <T> ::transpose() const
{
	return Matrix <T> (__cols, __rows,
		[this] __host__ __device__ (size_t i, size_t j) {
			return this->__array[j * __cols + i];
		}
	);
}

template <class T>
__host__ __device__
void Matrix <T> ::operator+=(const Matrix <T> &other)
{
	assert(__rows == other.__rows && __cols == other.__cols);

	for (size_t i = 0; i < __rows; i++) {
		for (size_t j = 0; j < __cols; j++)
			this->__array[i * __cols + j] += other.__array[i * __cols + j];
	}
}

template <class T>
__host__ __device__
void Matrix <T> ::operator-=(const Matrix <T> &other)
{
	assert(__rows == other.__rows && __cols == other.__cols);

	for (size_t i = 0; i < __rows; i++) {
		for (size_t j = 0; j < __cols; j++)
			this->__array[i * __cols + j] -= other.__array[i * __cols + j];
	}
}

template <class T>
__host__ __device__
void Matrix <T> ::operator*=(const T &x)
{
	for (size_t i = 0; i < this->__size; i++)
		this->__array[i] *= x;
}

template <class T>
__host__ __device__
void Matrix <T> ::operator/=(const T &x)
{
	for (size_t i = 0; i < this->__size; i++)
		this->__array[i] /= x;
}

// Matrix non-member operators
template <class T>
__host__ __device__
Matrix <T> operator+(const Matrix <T> &a, const Matrix <T> &b)
{
	assert(a.__rows == b.__rows && a.__cols == b.__cols);
	return Matrix <T> (a.__rows, a.__cols,
		[&](size_t i, size_t j) {
			return a[i][j] + b[i][j];
		}
	);
}

template <class T>
__host__ __device__
Matrix <T> operator-(const Matrix <T> &a, const Matrix <T> &b)
{
	// assert(a.__rows == b.__rows && a.__cols == b.__cols);
	return Matrix <T> (a.__rows, a.__cols,
		[&](size_t i, size_t j) {
			return a[i][j] - b[i][j];
		}
	);
}

template <class T>
__host__ __device__
Matrix <T> operator*(const Matrix <T> &a, const Matrix <T> &b)
{

#ifdef __CUDA_ARCH__

	// Cache constants
	size_t rs = a.__rows;
	size_t cs = b.__cols;

	size_t kmax = a.__cols;


	// Fill out the matrix
	inline_init_mat(other, rs, cs);

	for (size_t i = 0; i < rs; i++) {
		for (size_t j = 0; j < cs; j++) {
			T acc = 0;

			for (size_t k = 0; k < kmax; k++)
				acc += a[i][k] * b[k][j];

			other.__array[i * cs + j] = acc;
		}
	}

	return other;

#else

	if (a.__cols != b.__rows)
		throw typename Matrix <T> ::dimension_mismatch();

	return Matrix <T> (a.__rows, b.__cols,
		[&](size_t i, size_t j) {
			T acc = 0;

			for (size_t k = 0; k < a.__cols; k++)
				acc += a[i][k] * b[k][j];

			return acc;
		}
	);

#endif

}

template <class T>
__host__ __device__
Matrix <T> operator*(const Matrix <T> &a, const T &scalar)
{
	return Matrix <T> (a.__rows, a.__cols,
		[&](size_t i, size_t j) {
			return a[i][j] * scalar;
		}
	);
}

template <class T>
__host__ __device__
Matrix <T> operator*(const T &scalar, const Matrix <T> &a)
{
	return a * scalar;
}

template <class T>
__host__ __device__
Matrix <T> operator/(const Matrix <T> &a, const T &scalar)
{
	return Matrix <T> (a.__rows, a.__cols,
		[&](size_t i, size_t j) {
			return a[i][j] / scalar;
		}
	);
}

template <class T>
__host__ __device__
Matrix <T> operator/(const T &scalar, const Matrix <T> &a)
{
	return a / scalar;
}

template <class T>
__host__ __device__
bool operator==(const Matrix <T> &a, const Matrix <T> &b)
{
	if ((a.__rows != b.__rows) || (a.__cols != b.__cols))
		return false;

	for (size_t i = 0; i  < a.__rows; i++) {
		for (size_t j = 0; j < a.__cols; j++) {
			if (a[i][j] != b[i][j])
				return false;
		}
	}

	return true;
}

template <class T>
__host__ __device__
Matrix <T> shur(const Matrix <T> &a, const Matrix <T> &b)
{

#ifndef __CUDA_ARCH__
	
	// TODO: Replace with better exception handling for the device
	if (!((a.get_rows() == b.get_rows())
		&& (a.get_cols() == b.get_cols())))
		throw typename Matrix <T> ::dimension_mismatch();

	return Matrix <T> (a.get_rows(), b.get_cols(),
		[&](size_t i, size_t j) {
			return a[i][j] * b[i][j];
		}
	);

#else

	// Cache constants
	size_t rs = a.__rows;
	size_t cs = a.__cols;

	size_t sz = rs * cs;

	// Fill out the matrix
	inline_init_mat(other, rs, cs);
	for (size_t i = 0; i < sz; i++)
		other.__array[i] = a.__array[i] * b.__array[i];

	return other;

#endif

}

}

#endif
