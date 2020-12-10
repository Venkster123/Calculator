#ifndef STD_OPTIMIZER_CLASSES_H_
#define STD_OPTIMIZER_CLASSES_H_

// Engine headers
#include <optimizer.hpp>

// Engine std module headers
#include <std/optimizer_functions.hpp>

namespace zhetapi {
		
	namespace ml {
		
		/*
		* All optimizer classes have inlined member functions for the same
		* reason that the activation classes are inlined. Obscure naming is
		* also done for the same reason.
		*/

		// Squared error
		template <class T>
		class __DSquaredError : public Optimizer <T> {
		public:
			Vector <T> operator()(const Vector <T> &comp, const Vector <T> &in) const {
				return __d_squared <T> (comp, in);
			}
		};

		template <class T>
		class SquaredError : public Optimizer <T> {
		public:
			Vector <T> operator()(const Vector <T> &comp, const Vector <T> &in) const {
				return __squared <T> (comp, in);
			}

			Optimizer <T> *derivative() const
			{
				return new __DSquaredError <T> ();
			}
		};

		// Mean squared error
		template <class T>
		class __DMeanSquaredError : public Optimizer <T> {
		public:

#ifndef ZHP_CUDA

			Vector <T> operator()(const Vector <T> &comp, const Vector <T> &in) const {
				return __d_mean_squared <T> (comp, in);
			}

#else

			__host__ __device__
			Vector <T> operator()(const Vector <T> &comp, const Vector <T> &in) const {
				return -T(2)/T(comp.size()) * (comp - in);
			}

#endif

		};

		template <class T>
		class MeanSquaredError : public Optimizer <T> {
		public:

#ifndef ZHP_CUDA
			
			Vector <T> operator()(const Vector <T> &comp, const Vector <T> &in) const {
				return __mean_squared <T> (comp, in);
			}

			Optimizer <T> *derivative() const
			{
				return new __DMeanSquaredError <T> ();
			}

#else

			__host__ __device__
			Vector <T> operator()(const Vector <T> &comp, const Vector <T> &in) const {
				T sum = 0;

				for (size_t i = 0; i < comp.size(); i++)
					sum += (comp[i] - in[i]) * (comp[i] - in[i]);
				
				return Vector <T> (1, sum / T(comp.size()));
			}

			__host__ __device__
			Optimizer <T> *derivative() const
			{

#ifndef __CUDA_ARCH__

				return new __DMeanSquaredError <T> ();

#else

				Optimizer <T> *opt;

				cudaMalloc((void **) &opt, sizeof(Optimizer <T>));

				*opt = __DMeanSquaredError <T> ();

				return opt;

#endif
				
			}

#endif

		};

	}

}

#endif
