#pragma once

#include "JuliaSet.h"

namespace ChaoticDynamicalSystems
{
	// classes
	template <class V> class MandelbrotSet : public virtual JuliaSet<V>
	{
		// member variables
	private:
		std::complex<double> m_seed;
		double m_seed_mod;

		// member functions
	public:
		MandelbrotSet()
		{
			m_seed = std::complex<double>(0.0, 0.0);
			m_seed_mod = 0.0;
		}
		MandelbrotSet(std::complex<double> z)
		{
			m_x = 0.0;
			m_y = 0.0;
			m_z = z;
			m_seed = z;
			RecalculateSeedMod();
		}
		virtual double GetData() const
		{
			int i = 0;
			while (i < c_max_iterations)
			{
				double new_z_real = (m_z.real() * m_z.real()) - (m_z.imag() * m_z.imag()) + m_seed.real();
				double new_z_imag = (2 * m_z.real() * m_z.imag()) + m_seed.imag();
				double new_z_mod = sqrt((new_z_real * new_z_real) + (new_z_imag * new_z_imag));
				if (new_z_mod > m_seed_mod)
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
		virtual void Populate(const NArray::Index& index, const V& value)
		{
			JuliaSet<V>::Populate(index, value);
			m_seed = value;
			RecalculateSeedMod();
		}
		void RecalculateSeedMod()
		{
			m_seed_mod = sqrt((m_seed.real() * m_seed.real()) + (m_seed.imag() * m_seed.imag()));
			if (m_seed_mod < 2.0)
				m_seed_mod = 2.0;
		}
		static void Start(NArray::NArraySupport::StorageOption storage_option, int image_width, int image_height, bool use_transform, std::fstream& output_file, NArray::Mask* multi_threading_mask_ptr)
		{
			output_file << "The Mandelbrot Set - a mapping from the Complex Plane to the Cartesian Plane using ";
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
					storage_type_out mandelbrot_set(storage_option, image_shape, stored_type_out(0.0), true);
					CalculateMandelbrotSet<storage_type_in, stored_type_in, storage_type_out, stored_type_out>(complex_plane, mandelbrot_set, multi_threading_mask_ptr);

					Visualise2D<storage_type_out, stored_type_out>(mandelbrot_set, NArraySupport::UseDataToColour);
					ch = Visualisation<storage_type_out, stored_type_out>::GetLastChar();
					if (ch == 'z')
					{
						Visualisation<storage_type_out, stored_type_out>::GetZoom(x1, x2, y1, y2, z1, z2);
					}
				}
				else
				{
					typedef ChaoticDynamicalSystems::MandelbrotSet<stored_type_in> stored_type_out;
					typedef NArrayType<stored_type_out>::Matrix storage_type_out;
					storage_type_out mandelbrot_set(storage_option, image_shape, stored_type_out(), true);
					PopulateValues<storage_type_in, stored_type_in, storage_type_out, stored_type_out>(complex_plane, mandelbrot_set, multi_threading_mask_ptr);
					
					Visualise2D<storage_type_out, stored_type_out>(mandelbrot_set, NArraySupport::UseDataToColour);
					ch = Visualisation<storage_type_out, stored_type_out>::GetLastChar();
					if (ch == 'z')
					{
						Visualisation<storage_type_out, stored_type_out>::GetZoom(x1, x2, y1, y2, z1, z2);
					}
				}

				if (ch == 'r')
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
}
