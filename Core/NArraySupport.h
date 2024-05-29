#pragma once

#include <atomic>
#include <mutex>
#include <thread>
#include <vector>

#include "../Comparison/IComparable.h"
#include "Index.h"
#include "Mask.h"
#include "Shape.h"

namespace NArray
{
	class NArraySupport
	{
		// member data
	public:
		enum InteractionMode
		{
			Passive,
			Rotate,
			Zoom,
		};
		enum RegionSpecifier
		{
			DIAG,
			LTR,
			UTR,
		};
		enum StorageOption
		{
			ContiguousInternal,
			ContiguousExternal,
			DistributedInternal,
		};
		enum VisualisationMode
		{
			UseDataToSize,
			UseDataToColour,
			UseDataToSizeAndColour,
		};
		class MultiThreading
		{
			// member data
		private:
			static std::mutex m_mutex;
			static unsigned int m_thread_count;
			static std::vector<std::thread*> m_threads;
			// member functions
		public:
			static void AssignNewThread(std::thread* t);
			static void DecrementThreadCount();
			static unsigned int GetThreadCount();
			static void JoinThreads();
		};
		static const unsigned int c_max_num_threads;
		// member functions
	public:
		static bool CompareIndexToMask(const Index& index, const Mask& mask);
		static std::string MultiThreadingStr(bool multi_threaded);
		static bool QueryIndex(const Index& index, IComparable<int>& comparator);
		static bool QueryShape(const Shape& shape, IComparable<int>& comparator);
		static std::string ReIndexStr(bool reindex);
		static std::string StorageOptionStr(StorageOption storage_option);
	};
}
