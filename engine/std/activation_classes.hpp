#ifndef STD_ACTIVATION_CLASSES_H_
#define STD_ACTIVATION_CLASSES_H_

// Engine headers
#include <activation.hpp>

// Engine std module headers
#include <std/activation_functions.hpp>

namespace zhetapi {
		
	namespace ml {

		/*
		* All activation classes have inlined member functions as the
		* operations they perform are very minimal. These activation classes
		* serve only as a wrapper for their underlying functions as well as a
		* means of extracting their derivatives.
		*
		* The obscure naming of the derivative classes is done to encourage
		* users to use the derivative member function.
		*/
		
		// Linear activation class
		template <class T>
		class __DLinear : public Activation <T> {
			T	__alpha;
		public:
			__DLinear(const T &alpha = T(1)) : __alpha(alpha) {}

			Vector <T> operator()(const Vector <T> &x) const {
				return Vector <T> (x.size(), __alpha);
			}
		};
		
		template <class T>
		class Linear : public Activation <T> {
			T	__alpha;
		public:
			Linear(const T &alpha = T(1)) : __alpha(alpha) {}

			Vector <T> operator()(const Vector <T> &x) const {
				return Vector <T> (x.size(), [&](size_t i) {return x[i]
						* __alpha;});
			}

			Activation <T> *derivative() const {
				return new __DLinear <T> (__alpha);
			}
		};

		// ReLU activation class
		template <class T>
		class __DReLU : public Activation <T> {
		public:
			Vector <T> operator()(const Vector <T> &x) const {
				Vector <T> tmp(x.size(), [&](size_t i) {return
						__d_relu(x[i]);});
				std::cout << "DReLU(" << x << ") = " << tmp << std::endl;
				return Vector <T> (x.size(), [&](size_t i) {return
						__d_relu(x[i]);});
			}
		};

		template <class T>
		class ReLU : public Activation <T> {
		public:
			Vector <T> operator()(const Vector <T> &x) const {
				return Vector <T> (x.size(), [&](size_t i) {return
						__relu(x[i]);});
			}

			Activation <T> *derivative() const {
				return new __DReLU <T> ();
			}
		};

		// Leaky ReLU activation class
		template <class T>
		class __DLeakyReLU : public Activation <T> {
			T	__alpha;
		public:
			__DLeakyReLU(const T &alpha) : __alpha(alpha) {}

			Vector <T> operator()(const Vector <T> &x) const {
				return Vector <T> (x.size(), [&](size_t i) {return
						__d_leaky_relu(x[i], __alpha);});
			}
		};
		
		template <class T>
		class LeakyReLU : public Activation <T> {
			T	__alpha;
		public:
			LeakyReLU(const T &alpha) : __alpha(alpha) {}

			Vector <T> operator()(const Vector <T> &x) const {
				return Vector <T> (x.size(), [&](size_t i) {return
						__leaky_relu(x[i], __alpha);});
			}

			Activation <T> *derivative() const {
				return new __DLeakyReLU <T> (__alpha);
			}
		};

		// Sigmoid activation class
		template <class T>
		class __DSigmoid : public Activation <T> {
		public:
			Vector <T> operator()(const Vector <T> &x) const {
				return Vector <T> (x.size(), [&](size_t i) {return
						__d_sigmoid(x[i]);});
			}
		};

		template <class T>
		class Sigmoid : public Activation <T> {
		public:
			Vector <T> operator()(const Vector <T> &x) const {
				return Vector <T> (x.size(), [&](size_t i) {return
						__sigmoid(x[i]);});
			}

			Activation <T> *derivative() const {
				return new __DSigmoid <T> ();
			}
		};

		// Scaled Sigmoid activation class
		template <class T>
		class __DScaledSigmoid : public Activation <T> {
			T	__alpha;
		public:
			__DScaledSigmoid(const T &alpha) : __alpha(alpha) {}

			Vector <T> operator()(const Vector <T> &x) const {
				return Vector <T> (x.size(), [&](size_t i) {return
						__d_scaled_sigmoid(x[i], __alpha);});
			}
		};
		
		template <class T>
		class ScaledSigmoid : public Activation <T> {
			T	__alpha;
		public:
			ScaledSigmoid(const T &alpha) : __alpha(alpha) {}

			Vector <T> operator()(const Vector <T> &x) const {
				return Vector <T> (x.size(), [&](size_t i) {return
						__scaled_sigmoid(x[i], __alpha);});
			}

			Activation <T> *derivative() const {
				return new __DScaledSigmoid <T> (__alpha);
			}
		};

		// Probability activation class
		template <class T>
		class __DSoftmax : public Activation <T> {
		public:
			Vector <T> operator()(const Vector <T> &x) const {
				// Subtract by max for numerical stability
				T _max = x[0];
				for (size_t i = 1; i < x.size(); i++)
					_max = (_max > x[i]) ? _max : x[i];

				T _sum;
				for (size_t i = 0; i < x.size(); i++)
					_sum += exp(x[i] - _max);

				return Vector <T> (x.size(),
					[&](size_t i) {
						return exp(x[i] - _max)
							* (_sum - exp(x[i] - _max))
							/ (_sum * _sum);
					}
				);
			}
		};

		template <class T>
		class Softmax : public Activation <T> {
		public:
			Vector <T> operator()(const Vector <T> &x) const {
				// Subtract by max for numerical stability
				T _max = x[0];
				for (size_t i = 1; i < x.size(); i++)
					_max = (_max > x[i]) ? _max : x[i];

				T _sum;
				for (size_t i = 0; i < x.size(); i++)
					_sum += exp(x[i] - _max);

				return Vector <T> (x.size(),
					[&](size_t i) {
						return exp(x[i] - _max)/_sum;
					}
				);
			}

			Activation <T> *derivative() const {
				return new __DSoftmax <T> ();
			}
		};

	}

}

#endif
