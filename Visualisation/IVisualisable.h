#pragma once

namespace NArray
{
	// interface for ensuring an object can be visualised
	class IVisualisable abstract
	{
		// member functions
	public:
		virtual double GetColour() const = 0;
	};

	// interface for ensuring an object can be visualised in 0D
	class IVisualisable0D abstract : public virtual IVisualisable
	{
		// member functions
	public:
		virtual double GetData() const = 0;
		virtual double GetMaxData() const = 0;
		virtual double GetMinData() const = 0;
		virtual double GetScaledData() const = 0;
	};

	// interface for ensuring an object can be visualised in 1D
	class IVisualisable1D abstract : public virtual IVisualisable0D
	{
		// member functions
	public:
		virtual double GetMaxX() const = 0;
		virtual double GetMinX() const = 0;
		virtual double GetScaledX() const = 0;
		virtual double GetX() const = 0;
	};

	// interface for ensuring an object can be visualised in 2D
	class IVisualisable2D abstract : public virtual IVisualisable1D
	{
		// member functions
	public:
		virtual double GetMaxY() const = 0;
		virtual double GetMinY() const = 0;
		virtual double GetScaledY() const = 0;
		virtual double GetY() const = 0;
	};

	// interface for ensuring an object can be visualised in 3D
	class IVisualisable3D abstract : public virtual IVisualisable2D
	{
		// member functions
	public:
		virtual double GetMaxZ() const = 0;
		virtual double GetMinZ() const = 0;
		virtual double GetScaledZ() const = 0;
		virtual double GetZ() const = 0;
	};
}
