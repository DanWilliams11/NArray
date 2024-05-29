#pragma once

#include "../Visualisation/IVisualisable.h"

namespace NArray
{
	// interface for ensuring an object can be animated
	class IAnimatable abstract : public virtual IVisualisable
	{
		// member functions
	public:
		virtual void Update() const = 0;
	};

	// interface for ensuring an object can be animated in 0D
	class IAnimatable0D abstract : public virtual IVisualisable0D, public virtual IAnimatable
	{
	};

	// interface for ensuring an object can be animated in 1D
	class IAnimatable1D abstract : public virtual IVisualisable1D, public virtual IAnimatable
	{
	};

	// interface for ensuring an object can be animated in 2D
	class IAnimatable2D abstract : public virtual IVisualisable2D, public virtual IAnimatable
	{
	};

	// interface for ensuring an object can be animated in 3D
	class IAnimatable3D abstract : public virtual IVisualisable3D, public virtual IAnimatable
	{
	};
}
