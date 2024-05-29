#pragma once

#include <complex>

#include "../Core/NArrayDefinition.h"
#include "../Visualisation/IPopulatable.h"
#include "../Visualisation/IVisualisable.h"

namespace ChaoticDynamicalSystems
{
	// classes
	template <class V> class JuliaSet : public virtual NArray::IVisualisable2D, public virtual NArray::IPopulatable<V>
	{
		// member variables
	private:
		static std::complex<double> s_seed;
		static double s_seed_mod;
	protected:
		static const int c_max_iterations = 360;
		double m_x;
		double m_y;
		mutable std::complex<double> m_z;

		// member functions
	public:
		JuliaSet()
		{
			m_x = 0.0;
			m_y = 0.0;
			m_z = std::complex<double>(0.0, 0.0);
		}
		JuliaSet(std::complex<double> seed, std::complex<double> z)
		{
			s_seed = seed;
			RecalculateSeedMod();
			m_x = 0.0;
			m_y = 0.0;
			m_z = z;
		}
		virtual double GetColour() const
		{
			return 0.0;
		}
		virtual double GetData() const
		{
			int i = 0;
			while (i < c_max_iterations)
			{
				double new_z_real = (m_z.real() * m_z.real()) - (m_z.imag() * m_z.imag()) + s_seed.real();
				double new_z_imag = (2 * m_z.real() * m_z.imag()) + s_seed.imag();
				double new_z_mod = sqrt((new_z_real * new_z_real) + (new_z_imag * new_z_imag));
				if (new_z_mod > s_seed_mod)
				{
					break;
				}
				m_z.real(new_z_real);
				m_z.imag(new_z_imag);
				i++;
			}
			if (i < 20)
				i = 0;
			return i;
		}
		virtual double GetMaxData() const
		{ 
			return c_max_iterations;
		}
		virtual double GetMaxX() const
		{ 
			return 1000.0;
		}
		virtual double GetMaxY() const
		{ 
			return 1000.0;
		}
		virtual double GetMinData() const
		{ 
			return 0.0;
		}
		virtual double GetMinX() const
		{ 
			return 0.0;
		}
		virtual double GetMinY() const
		{ 
			return 0.0;
		}
		virtual double GetScaledData() const
		{
			return GetData();
		}
		virtual double GetScaledX() const
		{
			return (GetX() - GetMinX()) / (GetMaxX() - GetMinX());
		}
		virtual double GetScaledY() const
		{
			return (GetY() - GetMinY()) / (GetMaxY() - GetMinY());
		}
		virtual double GetX() const
		{
			return m_x;
		}
		virtual double GetY() const
		{
			return m_y;
		}
		virtual void Populate(const NArray::Index& index, const V& value)
		{
			m_x = index.GetIndexAtDim(2);
			m_y = index.GetIndexAtDim(1);
			m_z = value;
		}
		static void RecalculateSeedMod()
		{
			s_seed_mod = sqrt((s_seed.real() * s_seed.real()) + (s_seed.imag() * s_seed.imag()));
			if (s_seed_mod < 2.0)
				s_seed_mod = 2.0;
		}
		static void Start(NArray::NArraySupport::StorageOption storage_option, int image_width, int image_height, bool use_transform, std::fstream& output_file, NArray::Mask* multi_threading_mask_ptr)
		{
			output_file << "The Julia Set - a mapping from the Complex Plane to the Cartesian Plane using ";
			if (use_transform)
				output_file << "a Transform" << std::endl << std::endl;
			else
				output_file << "IVisualisable" << std::endl << std::endl;

			double x1 = -2.0;
			double x2 = 2.0;
			double y1 = -2.0;
			double y2 = 2.0;
			double z1 = 0.0;
			double z2 = 0.0;
			s_seed.real(-0.75);
			s_seed.imag(0.1);
			RecalculateSeedMod();

			char ch = '1';
			while (ch != 'q')
			{
				typedef std::complex<double> stored_type_in;
				typedef NArrayType<stored_type_in>::Matrix storage_type_in;
				stored_type_in start_val(x1, y1);
				stored_type_in end_val(x2, y2);
				stored_type_in step_val(0.0, 0.0);
				step_val.real((end_val.real() - start_val.real()) / ((double)image_width));
				step_val.imag((end_val.imag() - start_val.imag()) / ((double)image_height));
				Shape image_shape(c_matrix_order, image_height, image_width);
				storage_type_in complex_plane(storage_option, image_shape, stored_type_in(0.0, 0.0), true);
				MakeComplexPlane<storage_type_in, stored_type_in>(complex_plane, start_val, step_val, multi_threading_mask_ptr);

				if (use_transform)
				{
					typedef double stored_type_out;
					typedef NArrayType<stored_type_out>::Matrix storage_type_out;
					storage_type_out julia_set(storage_option, image_shape, stored_type_out(0.0), true);
					CalculateJuliaSet<storage_type_in, stored_type_in, storage_type_out, stored_type_out>(complex_plane, julia_set, s_seed, multi_threading_mask_ptr);

					Visualise2D<storage_type_out, stored_type_out>(julia_set, NArraySupport::UseDataToColour);
					ch = Visualisation<storage_type_out, stored_type_out>::GetLastChar();
					if (ch == 'z')
					{
						Visualisation<storage_type_out, stored_type_out>::GetZoom(x1, x2, y1, y2, z1, z2);
					}
				}
				else
				{
					typedef ChaoticDynamicalSystems::JuliaSet<stored_type_in> stored_type_out;
					typedef NArrayType<stored_type_out>::Matrix storage_type_out;
					storage_type_out julia_set(storage_option, image_shape, stored_type_out(), true);
					PopulateValues<storage_type_in, stored_type_in, storage_type_out, stored_type_out>(complex_plane, julia_set, multi_threading_mask_ptr);
					
					Visualise2D<storage_type_out, stored_type_out>(julia_set, NArraySupport::UseDataToColour);
					ch = Visualisation<storage_type_out, stored_type_out>::GetLastChar();
					if (ch == 'z')
					{
						Visualisation<storage_type_out, stored_type_out>::GetZoom(x1, x2, y1, y2, z1, z2);
					}
				}

				if (ch == '1')
				{
					s_seed.real(-0.75);
					s_seed.imag(0.1);
					RecalculateSeedMod();
				}
				else if (ch == '2')
				{
					s_seed.real(0.360284);
					s_seed.imag(0.100376);
					RecalculateSeedMod();
				}
				else if (ch == '3')
				{
					s_seed.real(-1.0);
					s_seed.imag(0.0);
					RecalculateSeedMod();
				}
				else if (ch == '4')
				{
					s_seed.real(-0.66753135776848660);
					s_seed.imag(0.32609027375103006);
					RecalculateSeedMod();
				}
				else if (ch == 'n')
				{
					std::complex<double> min_seed(-1.0, -1.0);
					std::complex<double> max_seed(1.0, 1.0);
					NArrayType<std::complex<double>>::Scalar random_seed(NArraySupport::ContiguousInternal, min_seed);
					MakeRandom<NArrayType<std::complex<double>>::Scalar, std::complex<double>>(random_seed, min_seed, max_seed);
					s_seed = random_seed.GetValue();
					RecalculateSeedMod();
				}
				else if (ch == 'r')
				{
					x1 = -2.0;
					x2 = 2.0;
					y1 = -2.0;
					y2 = 2.0;
					z1 = 0.0;
					z2 = 0.0;
				}
			}
		}
	};

	// static member variables
	template <class V> std::complex<double> JuliaSet<V>::s_seed(0.0, 0.0);
	template <class V> double JuliaSet<V>::s_seed_mod = 0.0;

	// non-member operators
	template <class V> std::ostream& operator <<(std::ostream& output_stream, const JuliaSet<V>& in)
	{
		in; // do this to prevent compiler warning 'unreferenced formal parameter'
		return output_stream;
	}
	template <class V> std::istream& operator >>(std::istream& input_stream, JuliaSet<V>& in)
	{
		in; // do this to prevent compiler warning 'unreferenced formal parameter'
		return input_stream;
	}
}
