// maximalsquare.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include "maximalsquare.h"


COccupyCollision OccupyCollision;



int main()
{
	//==================================================================
	// y
	// ^
	// |	+--- right,top
	// |	|         |
	// |	|         | 
	// | left,bottom--+
	// +--------------------------> x
	//==================================================================

	auto    nMapSide	= 100;						// Square Map Size 
	OccupyCollision.InitInstance(nMapSide);			// Create Map

	auto    nBlockSide  = 10;
	auto	nLineBlock  = (nMapSide / nBlockSide);
	auto	nTotalBlock = nLineBlock * nLineBlock;

	//==================================================================
	std::cout << "parallel_for START!\n";
	//==================================================================
	std::atomic<Int32>	Batched = 0;				// Success Batch Count

	auto	nBlockBatch = (nBlockSide / 2);			// Batch Count Per Block
	auto	nTotalBatch = nTotalBlock * nBlockBatch;// Total Batch Count

	auto start = clock();

	concurrency::parallel_for(0, nTotalBlock, [&](int nBlock)
	//for( int nBlock = 0; nBlock < nTotalBlock; nBlock++)
	{
		Int32 x = nBlock % nLineBlock;				// Block X Index
		Int32 y = nBlock / nLineBlock;				// Block Y Index
		core::Rect	rect(x* nBlockSide, y* nBlockSide, nBlockSide);	// Block Rectangle

		for (Int32 n = 0; n < (Int32)nBlockBatch; ++n)
		{
			Int32 occupyVolume = 1;					// Physical Volume	(Real Occupied Volume)
			Int32 searchVolume = occupyVolume * 2;	// Search   Volume	(For Avoiding Adjacent Batch)
			core::Point point;

			while (searchVolume >= occupyVolume)
			{
				if (OccupyCollision.Search(rect, searchVolume, point, nullptr, nullptr) == false)
				{
					// Failed ? Reduce the Search Volume to the Occupy Volume (Step by Step)
					--searchVolume;
					continue;
				}

				// Occupy Map with Searched Point & Volume 
				if (OccupyCollision.Insert(point.x, point.y, occupyVolume))
				{
					++Batched;
				}
				break;
			}
		}
		
	}
	); // concurrency::parallel_for

	auto finish		= clock();
	auto duration	= (double)(finish - start) / CLOCKS_PER_SEC;

	OccupyCollision.m_OccupyCollision.Dump();

	//==================================================================
	std::cout << "parallel_for END! : SUCCESS " << Batched << " : FAILED " << (nTotalBatch - Batched) << "\n";

	std::cout << duration << "ÃÊ" << std::endl;
	//==================================================================


}
