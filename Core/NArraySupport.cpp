#include "NArraySupport.h"

using namespace NArray;

const unsigned int NArraySupport::c_max_num_threads = std::thread::hardware_concurrency();
std::mutex NArraySupport::MultiThreading::m_mutex;
unsigned int NArraySupport::MultiThreading::m_thread_count = 0;
std::vector<std::thread*> NArraySupport::MultiThreading::m_threads;

void NArraySupport::MultiThreading::AssignNewThread(std::thread* t)
{
	m_mutex.lock();
	m_threads.emplace_back(t);
	m_thread_count++;
	m_mutex.unlock();
}

void NArraySupport::MultiThreading::DecrementThreadCount()
{
	m_mutex.lock();
	m_thread_count--;
	m_mutex.unlock();
}

unsigned int NArraySupport::MultiThreading::GetThreadCount()
{
	m_mutex.lock();
	unsigned int thread_count = m_thread_count;
	m_mutex.unlock();
	return thread_count;
};

void NArraySupport::MultiThreading::JoinThreads()
{
	m_mutex.lock();
	for (auto& t : m_threads)
	{
		t->join();
		delete t;
	}
	m_threads.erase(m_threads.begin(), m_threads.end());
	m_mutex.unlock();
}

bool NArraySupport::CompareIndexToMask(const Index& index, const Mask& mask)
{
	bool same = false;
	if (index.GetOrder() == 0)
		same = true;
	else
	{
		for (int dim = 1; dim <= index.GetOrder(); dim++)
		{
			if ((mask.GetMaskAtDim(dim) == Mask::use_all) || (index.GetIndexAtDim(dim) == mask.GetMaskAtDim(dim)))
				same = true;
			else
			{
				same = false;
				break;
			}
		}
	}
	return same;
}

std::string NArraySupport::MultiThreadingStr(bool multi_threaded)
{
	std::string str = "single-threading";
	if (multi_threaded)
		str = "multi-threading";
	return str;
}

bool NArraySupport::QueryIndex(const Index& index, IComparable<int>& comparator)
{
	bool query = true;
	for (int dim = 1; dim < index.GetOrder(); dim++)
	{
		if (!comparator.ApplyComparator(index.GetIndexAtDim(dim), index.GetIndexAtDim(dim + 1)))
		{
			query = false;
			break;
		}
	}
	return query;
}

bool NArraySupport::QueryShape(const Shape& shape, IComparable<int>& comparator)
{
	bool query = true;
	for (int dim = 1; dim < shape.GetOrder(); dim++)
	{
		if (!comparator.ApplyComparator(shape.GetShapeAtDim(dim), shape.GetShapeAtDim(dim + 1)))
		{
			query = false;
			break;
		}
	}
	return query;
}

std::string NArraySupport::ReIndexStr(bool reindex)
{
	std::string str = "off";
	if (reindex)
		str = "on";
	return str;
}

std::string NArraySupport::StorageOptionStr(StorageOption storage_option)
{
	std::string str;
	switch (storage_option)
	{
	case NArraySupport::ContiguousInternal:
		str = "contiguous-internally";
		break;
	case NArraySupport::ContiguousExternal:
		str = "contiguous-externally";
		break;
	case NArraySupport::DistributedInternal:
		str = "distributed-internally";
		break;
	default:
		str = "unknown-storage-option";
		break;
	}
	return str;
}
