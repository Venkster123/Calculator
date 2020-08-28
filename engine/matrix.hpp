#ifndef MATRIX_H_
#define MATRIX_H_

#include <cassert>
#include <functional>
#include <utility>
#include <vector>
#include <iostream>
#include <sstream>

#ifdef minor

#undef minor

#endif

template <class T>
class Vector;

/**
 * @brief A general Matrix class
 * (could be a single row/col vector)
 * that supports conventional operations
 * that matrices in mathematics do.
 */
template <class T>
class Matrix {
protected:
	T **m_array;

	size_t rows;
	size_t cols;
public:
	Matrix();
	Matrix(const Matrix <T> &);

	Matrix(T);
	Matrix(T **);
	Matrix(const std::vector <T> &);
	Matrix(const std::vector <std::vector <T>> &);

	Matrix(const std::initializer_list <std::initializer_list <T>> &);

	Matrix(size_t, size_t, T = T());
	Matrix(size_t, size_t, T, T **);

	Matrix(size_t, size_t, std::function <T (size_t)>);
	Matrix(size_t, size_t, std::function <T *(size_t)>);
	
	Matrix(size_t, size_t, std::function <T (size_t, size_t)>);
	Matrix(size_t, size_t, std::function <T *(size_t, size_t)>);

	~Matrix();

	std::pair <size_t, size_t> get_dimensions() const;

	size_t get_rows() const;
	size_t get_cols() const;

	const Matrix &slice(const std::pair <size_t, size_t> &,
			const std::pair <size_t, size_t> &) const;

	Matrix slice(const std::pair <size_t, size_t> &,
			const std::pair <size_t, size_t> &);

	void set(size_t, size_t, T);

	const T &get(size_t, size_t) const;

	Vector <T> get_column(size_t) const;

	T *operator[](size_t);
	const T *operator[](size_t) const;

	// Concatenating matrices
	Matrix append_above(const Matrix &);
	Matrix append_below(const Matrix &);

	Matrix append_left(const Matrix &);
	Matrix append_right(const Matrix &);

	void operator+=(const Matrix &);
	void operator-=(const Matrix &);
	void operator*=(const Matrix &);
	void operator/=(const Matrix &);

	// Row operations
	void add_rows(size_t, size_t, T);
	
	void swap_rows(size_t, size_t);

	void multiply_row(size_t, T);

	// Miscellanious opertions
	void randomize(const std::function <T ()> &);

	// Values
	T determinant() const;

	T minor(size_t, size_t) const;
	T minor(const std::pair <size_t, size_t> &) const;

	T cofactor(size_t, size_t) const;
	T cofactor(const std::pair <size_t, size_t> &) const;

	const Matrix &inverse() const;
	const Matrix &adjugate() const;
	const Matrix &cofactor() const;
	const Matrix &transpose() const;

	bool symmetric() const;

	std::string display() const;
	std::string display_nice() const;

	template <class U>
	friend const Matrix <U> &operator+(const Matrix <U> &, const Matrix <U> &);
	
	template <class U>
	friend const Matrix <U> &operator-(const Matrix <U> &, const Matrix <U> &);

	template <class U>
	friend const Matrix <U> &operator*(const Matrix <U> &, const Matrix <U> &);

	/* template <class U>
	friend const U &operator*(const Matrix <U> &, const Matrix <U> &); */

	template <class U>
	friend const Matrix <U> &operator*(const Matrix <U> &, const U &);
	
	template <class U>
	friend const Matrix <U> &operator*(const U &, const Matrix <U> &);
	
	template <class U>
	friend const Matrix <U> &operator/(const Matrix <U> &, const U &);
	
	template <class U>
	friend const Matrix <U> &operator/(const U &, const Matrix <U> &);

	template <class U>
	friend bool operator==(const Matrix <U> &, const Matrix <U> &);

	template <class U>
	friend std::ostream &operator<<(std::ostream &, const Matrix <U> &);

	// Special matrix generation
	static Matrix identity(size_t);

	// Ostream settings
	static bool nice;
protected:
	T determinant(const Matrix &) const;
};

// Set static Variables
template <class T>
bool Matrix <T> ::nice = false;

template <class T>
Matrix <T> ::Matrix() : rows(0), cols(0), m_array(nullptr) {}

template <class T>
Matrix <T> ::Matrix(const Matrix <T> &other) : rows(other.rows),
		cols(other.cols)
{
	m_array = new T *[rows];

	for (int i = 0; i < rows; i++) {
		m_array[i] = new T[cols];

		for (int j = 0; j < cols; j++)
			m_array[i][j] = other[i][j];
	}
}

/*
 * Acts as a conversion from T to Matrix <T>, which is necessary in the Barn
 * class.
 */
template <class T>
Matrix <T> ::Matrix(T x) : Matrix({{x}}) {}

template <class T>
Matrix <T> ::Matrix(T **ref)
{
	rows = sizeof(ref)/sizeof(T);

	assert(rows > 0);

	cols = sizeof(ref[0])/sizeof(T);

	assert(cols > 0);

	m_array = new T *[rows];

	for (int i = 0; i < rows; i++) {
		m_array[i] = new T[cols];

		for (int j = 0; j < cols; j++)
			m_array[i][j] = ref[i][j];
	}
}

template <class T>
Matrix <T> ::Matrix(const std::vector <T> &ref)
{
	rows = ref.size();

	assert(rows > 0);

	cols = 1;
	
	m_array = new T *[rows];

	for (int i = 0; i < rows; i++)
		m_array[i] = new T {ref[i]};
}

template <class T>
Matrix <T> ::Matrix(const std::vector <std::vector <T>> &ref)
{
	rows = ref.size();

	assert(rows > 0);

	cols = ref[0].size();

	assert(cols > 0);
	
	m_array = new T *[rows];

	for (int i = 0; i < rows; i++) {
		m_array[i] = new T[cols];

		for (int j = 0; j < cols; j++) {
			assert(i < rows && j < ref[i].size());
			m_array[i][j] = ref[i][j];
		}
	}
}

template <class T>
Matrix <T> ::Matrix(const std::initializer_list <std::initializer_list <T>> &sq)
{
	rows = sq.size();

	assert(rows > 0);

	cols = sq.begin()->size();

	assert(cols > 0);
	
	m_array = new T *[rows];

	size_t i = 0;
	for (auto lt : sq) {
		m_array[i] = new T[cols];

		size_t j = 0;
		for (auto t : lt) {
			assert(i < rows && j < lt.size());
			m_array[i][j++] = t;
		}

		i++;
	}
}

template <class T>
Matrix <T> ::Matrix(size_t rs, size_t cs, T val)
{
	rows = rs;
	cols = cs;

	m_array = new T *[rows];

	for (int i = 0; i < rows; i++) {
		m_array[i] = new T[cols];
		
		for (int j = 0; j < cols; j++)
			m_array[i][j] = val;
	}
}

template <class T>
Matrix <T> ::Matrix(size_t rows, size_t cols, T val, T **ref)
{
}

template <class T>
Matrix <T> ::Matrix(size_t rs, size_t cs,
		std::function <T (size_t)> gen)
{
	rows = rs;
	cols = cs;

	m_array = new T *[rows];

	for (int i = 0; i < rows; i++) {
		m_array[i] = new T[cols];
		
		for (int j = 0; j < cols; j++)
			m_array[i][j] = gen(i);
	}
}

template <class T>
Matrix <T> ::Matrix(size_t rs, size_t cs,
		std::function <T *(size_t)> gen)
{
	rows = rs;
	cols = cs;

	m_array = new T *[rows];

	for (int i = 0; i < rows; i++) {
		m_array[i] = new T[cols];
		
		for (int j = 0; j < cols; j++)
			m_array[i][j] = *gen(i);
	}
}

template <class T>
Matrix <T> ::Matrix(size_t rs, size_t cs,
		std::function <T (size_t, size_t)> gen)
{
	rows = rs;
	cols = cs;

	m_array = new T *[rows];

	for (int i = 0; i < rows; i++) {
		m_array[i] = new T[cols];
		
		for (int j = 0; j < cols; j++)
			m_array[i][j] = gen(i, j);
	}
}

template <class T>
Matrix <T> ::Matrix(size_t rs, size_t cs,
		std::function <T *(size_t, size_t)> gen)
{
	rows = rs;
	cols = cs;

	m_array = new T *[rows];

	for (int i = 0; i < rows; i++) {
		m_array[i] = new T[cols];
		
		for (int j = 0; j < cols; j++)
			m_array[i][j] = *gen(i, j);
	}
}

template <class T>
Matrix <T> ::~Matrix()
{
	delete[] m_array;
}

template <class T>
std::pair <size_t, size_t> Matrix <T> ::get_dimensions() const
{
	return {rows, cols};
}

template <class T>
size_t Matrix <T> ::get_rows() const
{
	return rows;
}

template <class T>
size_t Matrix <T> ::get_cols() const
{
	return cols;
}

template <class T>
const Matrix <T> &Matrix <T> ::slice(const std::pair <size_t, size_t> &start,
		const std::pair <size_t, size_t> &end) const
{
	/* The following asserts make sure the pairs
	 * are in bounds of the Matrix and that they
	 * are in order */
	assert(start.first <= end.first && start.second <= end.second);

	assert(start.first < rows && start.second < cols);
	assert(end.first < rows && end.second < cols);

	/* Slicing is inclusive of the last
	 * Vector passed */
	Matrix <T> *out = new Matrix <T> (end.first - start.first + 1,
			end.second - start.second + 1, [&](size_t i, size_t j) {
			return m_array[i + start.first][j + start.second];
	});

	return *out;
}

template <class T>
Matrix <T> Matrix <T> ::slice(const std::pair <size_t, size_t> &start,
		const std::pair <size_t, size_t> &end)
{
	/* The following asserts make sure the pairs
	 * are in bounds of the Matrix and that they
	 * are in order */
	assert(start.first <= end.first && start.second <= end.second);

	assert(start.first < rows && start.second < cols);
	assert(end.first < rows && end.second < cols);

	/* Slicing is inclusive of the last
	 * Vector passed */
	Matrix <T> *out = new Matrix <T> (end.first - start.first + 1,
			end.second - start.second + 1, [&](size_t i, size_t j) {
			return m_array[i + start.first][j + start.second];
	});

	return *out;
}

template <class T>
void Matrix <T> ::set(size_t row, size_t col, T val)
{
	m_array[row][col] = val;
}

template <class T>
const T &Matrix <T> ::get(size_t row, size_t col) const
{
	return m_array[row][col];
}

template <class T>
Vector <T> Matrix <T> ::get_column(size_t n) const
{
	return Vector <T> (rows, [&](size_t i) {
		return m_array[i][n];
	});
}

template <class T>
T *Matrix <T> ::operator[](size_t i)
{
	return m_array[i];
}

template <class T>
const T *Matrix <T> ::operator[](size_t i) const
{
	return m_array[i];
}

template <class T>
Matrix <T> Matrix <T> ::append_above(const Matrix &m)
{
	assert(cols == m.cols);

	size_t t_rows = rows;
	size_t m_rows = m.rows;

	std::vector <std::vector <T>> row;

	std::vector <T> total;

	for (size_t i = 0; i < m_rows; i++) {
		total.clear();

		for (size_t j = 0; j < cols; j++)
			total.push_back(m[i][j]);

		row.push_back(total);
	}

	for (size_t i = 0; i < t_rows; i++) {
		total.clear();

		for (size_t j = 0; j < cols; j++)
			total.push_back(m_array[i][j]);

		row.push_back(total);
	}

	return Matrix(row);
}

template <class T>
Matrix <T> Matrix <T> ::append_below(const Matrix &m)
{
	assert(cols == m.cols);

	size_t t_rows = rows;
	size_t m_rows = m.rows;

	std::vector <std::vector <T>> row;

	std::vector <T> total;

	for (size_t i = 0; i < t_rows; i++) {
		total.clear();

		for (size_t j = 0; j < cols; j++)
			total.push_back(m_array[i][j]);

		row.push_back(total);
	}

	for (size_t i = 0; i < m_rows; i++) {
		total.clear();

		for (size_t j = 0; j < cols; j++)
			total.push_back(m[i][j]);

		row.push_back(total);
	}

	return Matrix(row);
}

template <class T>
Matrix <T> Matrix <T> ::append_left(const Matrix &m)
{
	assert(rows == m.rows);

	size_t t_cols = cols;
	size_t m_cols = m.cols;

	std::vector <std::vector <T>> row;

	std::vector <T> total;

	for (size_t i = 0; i < rows; i++) {
		total.clear();

		for (size_t j = 0; j < m_cols; j++)
			total.push_back(m[i][j]);

		row.push_back(total);
	}

	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; j < t_cols; j++)
			row[i].push_back(m_array[i][j]);
	}

	return Matrix(row);
}

template <class T>
Matrix <T> Matrix <T> ::append_right(const Matrix &m)
{
	assert(rows == m.rows);

	size_t t_cols = cols;
	size_t m_cols = m.cols;

	std::vector <std::vector <T>> row;

	std::vector <T> total;

	for (size_t i = 0; i < rows; i++) {
		total.clear();

		for (size_t j = 0; j < t_cols; j++)
			total.push_back(m_array[i][j]);

		row.push_back(total);
	}

	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; j < m_cols; j++)
			row[i].push_back(m[i][j]);
	}

	return Matrix(row);
}

template <class T>
void Matrix <T> ::operator+=(const Matrix <T> &other)
{
	assert(rows == other.rows && cols == other.cols);

	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; j < cols; j++)
			m_array[i][j] += other.m_array[i][j];
	}
}

template <class T>
void Matrix <T> ::operator-=(const Matrix <T> &other)
{
	assert(rows == other.rows && cols == other.cols);

	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; j < cols; j++)
			m_array[i][j] -= other.m_array[i][j];
	}
}

template <class T>
void Matrix <T> ::operator*=(const Matrix <T> &other)
{
	(*this) = (*this) * other;
}

// R(A) = R(A) + kR(B)
template <class T>
void Matrix <T> ::add_rows(size_t a, size_t b, T k)
{
	for (size_t i = 0; i < cols; i++)
		m_array[a][i] += k * m_array[b][i];
}

template <class T>
void Matrix <T> ::swap_rows(size_t a, size_t b)
{
	std::swap(m_array[a], m_array[b]);
}

template <class T>
void Matrix <T> ::multiply_row(size_t a, T k)
{
	for (size_t i = 0; i < cols; i++)
		m_array[a][i] *= k;
}

template <class T>
void Matrix <T> ::randomize(const std::function<T ()> &ftr)
{
	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; j < cols; j++)
			m_array[i][j] = ftr();
	}
}

template <class T>
T Matrix <T> ::determinant() const
{
	return determinant(*this);
}

template <class T>
T Matrix <T> ::minor(const std::pair <size_t, size_t> &pr) const
{
	Matrix <T> *out = new Matrix <T> (rows - 1, cols - 1);

	size_t a;
	size_t b;

	a = 0;
	for (size_t i = 0; i < rows; i++) {
		b = 0;
		if (i == pr.first)
			continue;

		for (size_t j = 0; j < cols; j++) {
			if (j == pr.second)
				continue;

			(*out)[a][b++] = m_array[i][j];
		}

		a++;
	}

	T min = determinant(*out);

	delete out;

	return min;
}

template <class T>
T Matrix <T> ::minor(size_t i, size_t j) const
{
	return minor({i, j});
}

template <class T>
T Matrix <T> ::cofactor(const std::pair <size_t, size_t> &pr) const
{
	return (((pr.first + pr.second) % 2) ? -1 : 1) * minor(pr);
}

template <class T>
T Matrix <T> ::cofactor(size_t i, size_t j) const
{
	return cofactor({i, j});
}

template <class T>
const Matrix <T> &Matrix <T> ::inverse() const
{
	return adjugate() / determinant();
}

template <class T>
const Matrix <T> &Matrix <T> ::adjugate() const
{
	return cofactor().transpose();
}

template <class T>
const Matrix <T> &Matrix <T> ::cofactor() const
{
	Matrix <T> *out = new Matrix(rows, cols, [&](size_t i, size_t j) {
		return cofactor(i, j);
	});

	return *out;
}

template <class T>
const Matrix <T> &Matrix <T> ::transpose() const
{
	Matrix <T> *out = new Matrix <T> (cols, rows, [&](size_t i, size_t j) {
		return m_array[j][i];
	});

	return *out;
}

template <class T>
const Matrix <T> &shur(const Matrix <T> &a, const Matrix <T> &b)
{
	assert(a.get_rows() == b.get_rows() && a.get_cols() == b.get_cols());

	Matrix <T> *out = new Matrix <T> (a.get_rows(), b.get_cols(), [&](size_t i, size_t j) {
		return a[i][j] * b[i][j];
	});

	return *out;
}

template <class T>
bool Matrix <T> ::symmetric() const
{
	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; j < cols; j++) {
			if (m_array[i][j] != m_array[j][i])
				return false;
		}
	}

	return true;
}

template <class T>
std::string Matrix <T> ::display() const
{
	std::ostringstream oss;

	oss << "[";

	for (int i = 0; i < rows; i++) {
		if (cols > 1) {
			oss << '[';

			for (int j = 0; j < cols; j++) {
				oss << m_array[i][j];
				if (j != cols - 1)
					oss << ", ";
			}

			oss << ']';
		} else {
			oss << m_array[i][0];
		}

		if (i < rows - 1)
			oss << ", ";
	}

	oss << "]";

	return oss.str();
}

template <class T>
std::string Matrix <T> ::display_nice() const
{
	std::ostringstream oss;
	for (int i = 0; i < rows; i++) {
		oss << '|';

		for (int j = 0; j < cols; j++) {
			oss << m_array[i][j];
			if (j != cols - 1)
				oss << "\t";
		}

		oss << '|';

		if (i < rows - 1)
			oss << "\n";
	}

	return oss.str();
}

template <class T>
const Matrix <T> &operator+(const Matrix <T> &a, const Matrix <T> &b)
{
	assert(a.rows == b.rows && a.cols == b.cols);
	
	Matrix <T> *out = new Matrix <T> (a.rows, a.cols, [&](size_t i, size_t j) {
		return a[i][j] + b[i][j];
	});

	return *out;
}

template <class T>
const Matrix <T> &operator-(const Matrix <T> &a, const Matrix <T> &b)
{
	assert(a.rows == b.rows && a.cols == b.cols);

	Matrix <T> *out = new Matrix <T> (a.rows, a.cols, [&](size_t i, size_t j) {
		return a[i][j] - b[i][j];
	});

	return *out;
}

template <class T>
const Matrix <T> &operator*(const Matrix <T> &a, const Matrix <T> &b)
{
	assert(a.cols == b.rows);

	Matrix <T> *out = new Matrix <T> (a.rows, b.cols, [&](size_t i, size_t j) {
		T acc = 0;

		for (size_t k = 0; k < a.cols; k++) {
			acc += a[i][k] * b[k][j];
		}

		return acc;
	});

	return *out;
}

/* template <class T>
const T &Matrix <T> ::operator*(const Matrix <T> &a, const Matrix <T> &b)
{
	assert(a.cols == b.rows && a.rows == b.cols == 1);
	return (a * b)[0][0];
} */

template <class T>
const Matrix <T> &operator*(const Matrix <T> &a, const T &scalar)
{
	Matrix <T> *out = new Matrix <T> (a.rows, a.cols, [&](size_t i, size_t j) {
		return a[i][j] * scalar;
	});

	return *out;
}

template <class T>
const Matrix <T> &operator*(const T &scalar, const Matrix <T> &a)
{
	return a * scalar;
}

template <class T>
const Matrix <T> &operator/(const Matrix <T> &a, const T &scalar)
{
	Matrix <T> *out = new Matrix <T> (a.rows, a.cols, [&](size_t i, size_t j) {
		return a[i][j] / scalar;
	});

	return *out;
}

template <class T>
const Matrix <T> &operator/(const T &scalar, const Matrix <T> &a)
{
	return a / scalar;
}

template <class T>
bool operator==(const Matrix <T> &a, const Matrix <T> &b)
{
	if (a.get_dimensions() != b.get_dimensions())
		return false;

	for (size_t i = 0; i  < a.get_rows(); i++) {
		for (size_t j = 0; j < a.get_cols(); j++) {
			if (a[i][j] != b[i][j])
				return false;
		}
	}

	return true;
}

template <class T>
Matrix <T> Matrix <T> ::identity(size_t dim)
{
	return Matrix {dim, dim, [](size_t i, size_t j) {
		if (i == j)
			return T(1);

		return T(0);
	}};
}

// Private helper methods
template <class T>
T Matrix <T> ::determinant(const Matrix <T> &a) const
{
	/* The determinant of an abitrary
	 * Matrix is defined only if it
	 * is a square Matrix */
	assert(a.rows == a.cols && a.rows > 0);

	size_t n;
	size_t t;
	
	n = a.rows;

	if (n == 1)
		return a[0][0];
	if (n == 2)
		return a[0][0] * a[1][1] - a[1][0] * a[0][1];

	T det = 0;

	Matrix <T> *temp;
	for (size_t i = 0; i < n; i++) {
		temp = new Matrix <T> (n - 1, n - 1);

		for (size_t j = 0; j < n - 1; j++) {
			t = 0;

			for (size_t k = 0; k < n; k++) {
				if (k == i)
					continue;
				(*temp)[j][t++] = a[j + 1][k];
			}
		}

		det += ((i % 2) ? -1 : 1) * a[0][i] * determinant(*temp);

		delete temp;
	}

	return det;
}

// Ostream
template <class T>
std::ostream &operator<<(std::ostream &os, const Matrix <T> &a)
{
	if (Matrix <T> ::nice)
		os << a.display_nice();
	else
		os << a.display();

	return os;
}

#endif