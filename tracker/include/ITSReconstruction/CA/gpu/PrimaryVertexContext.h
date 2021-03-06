// Copyright CERN and copyright holders of ALICE O2. This software is
// distributed under the terms of the GNU General Public License v3 (GPL
// Version 3), copied verbatim in the file "COPYING".
//
// See http://alice-o2.web.cern.ch/license for full licensing information.
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
///
/// \file PrimaryVertexContext.h
/// \brief
///

#include "ITSReconstruction/CA/Cell.h"
#include "ITSReconstruction/CA/Cluster.h"
#include "ITSReconstruction/CA/Constants.h"
#include "ITSReconstruction/CA/Definitions.h"
#include "ITSReconstruction/CA/Tracklet.h"
#if TRACKINGITSU_CUDA_MODE
#include "ITSReconstruction/CA/gpu/Array.h"
#include "ITSReconstruction/CA/gpu/UniquePointer.h"
#include "ITSReconstruction/CA/gpu/Vector.h"
#elif TRACKINGITSU_OCL_MODE
#include "ITSReconstruction/CA/gpu/StructGPUPrimaryVertex.h"
#include "ITSReconstruction/CA/Event.h"
#endif



namespace o2
{
namespace ITS
{
namespace CA
{
namespace GPU
{

class PrimaryVertexContext
  final
  {
    public:
      PrimaryVertexContext();
#if TRACKINGITSU_CUDA_MODE
      UniquePointer<PrimaryVertexContext> initialize(const float3&,
          const std::array<std::vector<Cluster>, Constants::ITS::LayersNumber>&,
          const std::array<std::vector<Cell>, Constants::ITS::CellsPerRoad>&,
          const std::array<std::vector<int>, Constants::ITS::CellsPerRoad - 1>&);
      GPU_DEVICE const float3& getPrimaryVertex();
      GPU_HOST_DEVICE Array<Vector<Cluster>,
          Constants::ITS::LayersNumber>& getClusters();
      GPU_DEVICE Array<Array<int, Constants::IndexTable::ZBins * Constants::IndexTable::PhiBins + 1>,
          Constants::ITS::TrackletsPerRoad>& getIndexTables();
      GPU_HOST_DEVICE Array<Vector<Tracklet>,
          Constants::ITS::TrackletsPerRoad>& getTracklets();
      GPU_HOST_DEVICE Array<Vector<int>,
          Constants::ITS::CellsPerRoad>& getTrackletsLookupTable();
      GPU_HOST_DEVICE Array<Vector<int>,
          Constants::ITS::CellsPerRoad>& getTrackletsPerClusterTable();
      GPU_HOST_DEVICE Array<Vector<Cell>,
          Constants::ITS::CellsPerRoad>& getCells();
      GPU_HOST_DEVICE Array<Vector<int>,
          Constants::ITS::CellsPerRoad - 1>& getCellsLookupTable();
      GPU_HOST_DEVICE Array<Vector<int>,
          Constants::ITS::CellsPerRoad - 1>& getCellsPerTrackletTable();
     Array<Vector<int>, Constants::ITS::CellsPerRoad>& getTempTableArray();

    private:
      UniquePointer<float3> mPrimaryVertex;
      Array<Vector<Cluster>, Constants::ITS::LayersNumber> mClusters;
      Array<Array<int, Constants::IndexTable::ZBins * Constants::IndexTable::PhiBins + 1>,
          Constants::ITS::TrackletsPerRoad> mIndexTables;
      Array<Vector<Tracklet>, Constants::ITS::TrackletsPerRoad> mTracklets;
      Array<Vector<int>, Constants::ITS::CellsPerRoad> mTrackletsLookupTable;
      Array<Vector<int>, Constants::ITS::CellsPerRoad> mTrackletsPerClusterTable;
      Array<Vector<Cell>, Constants::ITS::CellsPerRoad> mCells;
      Array<Vector<int>, Constants::ITS::CellsPerRoad - 1> mCellsLookupTable;
      Array<Vector<int>, Constants::ITS::CellsPerRoad - 1> mCellsPerTrackletTable;
#endif

#if TRACKINGITSU_OCL_MODE
      void initialize(cl::Context oclContext);

      void sortClusters(int iLayer);
      void boostInitialize(
    		  const Event& event,
    		  float3& mPrimaryVertex,const std::array<std::vector<Cluster>, Constants::ITS::LayersNumber>& clusters,
    		  std::array<std::vector<int>, Constants::ITS::CellsPerRoad>& mTrackletsLookupTable);

      GPU_DEVICE const FLOAT3* getPrimaryVertex();

      GPU_HOST_DEVICE ClusterStruct** getClusters();
      GPU_HOST_DEVICE inline void addClusters(const float3 &primaryVertex, const Cluster& other, int iLayer,int iCluster);
      GPU_HOST_DEVICE TrackletStruct** getTracklets();
      GPU_HOST_DEVICE int** getTrackletsLookupTable();
      GPU_HOST_DEVICE int** getTrackletsPerClusterTable();

    public:
      	 int iInitialize=1;
         FLOAT3 mPrimaryVertex;
         cl::Buffer bPrimaryVertex;

         cl::Buffer bLayerIndex[Constants::ITS::LayersNumber];

         ClusterStruct* mClusters[Constants::ITS::LayersNumber]={NULL};
         cl::Buffer bClusters[Constants::ITS::LayersNumber];
         cl::Buffer bClustersSize;
         int iClusterSize[Constants::ITS::LayersNumber];
         int iClusterAllocatedSize[Constants::ITS::LayersNumber];

         int iIndexTableSize=Constants::IndexTable::ZBins * Constants::IndexTable::PhiBins + 1;
         int mIndexTables[Constants::ITS::TrackletsPerRoad][Constants::IndexTable::ZBins * Constants::IndexTable::PhiBins + 1];
         cl::Buffer bIndexTables[Constants::ITS::TrackletsPerRoad];


         TrackletStruct* mTracklets[Constants::ITS::TrackletsPerRoad]={NULL};
         cl::Buffer bTracklets[Constants::ITS::TrackletsPerRoad];
         int iTrackletSize[Constants::ITS::TrackletsPerRoad];
         int iTrackletFoundPerLayer[Constants::ITS::TrackletsPerRoad];
         int iTrackletAllocatedSize[Constants::ITS::TrackletsPerRoad];
         cl::Buffer bTrackletsSize;

         int* mTrackletsLookupTable[Constants::ITS::CellsPerRoad]={NULL};
         int iTrackletsLookupTableSize[Constants::ITS::CellsPerRoad];
         int iTrackletsLookupTableAllocatedSize[Constants::ITS::CellsPerRoad];
         cl::Buffer bTrackletsLookupTable[Constants::ITS::CellsPerRoad];


         int* mTrackletsPerClusterTable[Constants::ITS::CellsPerRoad];
         cl::Buffer bTrackletsFoundForLayer;
         cl::Buffer bCellsFoundForLayer;
         int* iCellsPerTrackletTable[Constants::ITS::CellsPerRoad];
         cl::Buffer bCellsFoundForTracklet;

         //std::array<std::vector<Cluster>, Constants::ITS::LayersNumber> mClustersVector;

         ////// cells //////
         CellStruct* mCells[Constants::ITS::CellsPerRoad]={NULL};
         cl::Buffer bCells[Constants::ITS::CellsPerRoad];
         int iCellSize[Constants::ITS::CellsPerRoad];
         cl::Buffer bCellSize;

         int *iCellsLookupTable[Constants::ITS::CellsPerRoad-1]={NULL};
         cl::Buffer bCellsLookupTable[Constants::ITS::CellsPerRoad-1];
    	 int iCellsLookupTableSize[Constants::ITS::CellsPerRoad-1];
    	 cl::Buffer bCellsLookupTableSize;

    	 int *mCellsNeighbours[Constants::ITS::CellsPerRoad - 1]={NULL};

    	 //boost
    	 compute::buffer boostPrimaryVertex;
    	 compute::buffer boostClusterSize;
    	 compute::buffer boostTrackletsFoundForLayer;
    	 //compute::buffer boostIndexTables[Constants::ITS::TrackletsPerRoad];
    	 std::array<compute::buffer,Constants::ITS::TrackletsPerRoad> boostLayerIndex;
    	 std::array<compute::vector<Cluster>, Constants::ITS::LayersNumber> boostClusters;
    	 std::array<compute::vector<int>, Constants::ITS::CellsPerRoad> boostTrackletsLookupTable;
    	 std::array<compute::vector<Tracklet>, Constants::ITS::TrackletsPerRoad> boostTracklets;
    	 std::array<compute::vector<Cell>, Constants::ITS::CellsPerRoad> boostCells;
    	 std::array<compute::vector<int>, Constants::ITS::CellsPerRoad - 1> boostCellsLookupTable;
    	 std::array<compute::vector<int>, Constants::ITS::TrackletsPerRoad> boostIndexTables;


#endif

  };


#if TRACKINGITSU_CUDA_MODE
  GPU_DEVICE inline const float3& PrimaryVertexContext::getPrimaryVertex()
  {
    return *mPrimaryVertex;
  }

  GPU_HOST_DEVICE inline Array<Vector<Cluster>, Constants::ITS::LayersNumber>& PrimaryVertexContext::getClusters()
  {
    return mClusters;
  }

  GPU_DEVICE inline Array<Array<int, Constants::IndexTable::ZBins * Constants::IndexTable::PhiBins + 1>,
      Constants::ITS::TrackletsPerRoad>& PrimaryVertexContext::getIndexTables()
  {
    return mIndexTables;
  }

  GPU_DEVICE inline Array<Vector<Tracklet>, Constants::ITS::TrackletsPerRoad>& PrimaryVertexContext::getTracklets()
  {
    return mTracklets;
  }

  GPU_DEVICE inline Array<Vector<int>, Constants::ITS::CellsPerRoad>& PrimaryVertexContext::getTrackletsLookupTable()
  {
    return mTrackletsLookupTable;
  }

  GPU_DEVICE inline Array<Vector<int>, Constants::ITS::CellsPerRoad>& PrimaryVertexContext::getTrackletsPerClusterTable()
  {
    return mTrackletsPerClusterTable;
  }

  GPU_HOST_DEVICE inline Array<Vector<Cell>, Constants::ITS::CellsPerRoad>& PrimaryVertexContext::getCells()
  {
    return mCells;
  }

  GPU_HOST_DEVICE inline Array<Vector<int>, Constants::ITS::CellsPerRoad - 1>& PrimaryVertexContext::getCellsLookupTable()
  {
    return mCellsLookupTable;
  }

  GPU_HOST_DEVICE inline Array<Vector<int>, Constants::ITS::CellsPerRoad - 1>& PrimaryVertexContext::getCellsPerTrackletTable()
  {
    return mCellsPerTrackletTable;
  }
#elif TRACKINGITSU_OCL_MODE
  inline const FLOAT3* PrimaryVertexContext::getPrimaryVertex()
    {
      return &mPrimaryVertex;
    }

    GPU_HOST_DEVICE inline ClusterStruct** PrimaryVertexContext::getClusters()
    {
      return mClusters;
    }

    GPU_HOST_DEVICE inline void PrimaryVertexContext::addClusters(const float3 &primaryVertex, const Cluster& other, int iLayer,int iCluster)
    {
  	  	mClusters[iLayer][iCluster].xCoordinate=other.xCoordinate;
  	  	mClusters[iLayer][iCluster].yCoordinate=other.yCoordinate;
  	  	mClusters[iLayer][iCluster].zCoordinate=other.zCoordinate;
  	  	mClusters[iLayer][iCluster].clusterId=other.clusterId;
  	  	mClusters[iLayer][iCluster].monteCarloId=other.monteCarloId;
  	  	mClusters[iLayer][iCluster].alphaAngle=other.alphaAngle;
  		mClusters[iLayer][iCluster].phiCoordinate=MathUtils::getNormalizedPhiCoordinate(MathUtils::calculatePhiCoordinate(other.xCoordinate - primaryVertex.x, other.yCoordinate - primaryVertex.y));
  		mClusters[iLayer][iCluster].rCoordinate=MathUtils::calculateRCoordinate(other.xCoordinate - primaryVertex.x, other.yCoordinate - primaryVertex.y);
  		mClusters[iLayer][iCluster].indexTableBinIndex=IndexTableUtils::getBinIndex(IndexTableUtils::getZBinIndex(iLayer, other.zCoordinate),IndexTableUtils::getPhiBinIndex(mClusters[iLayer][iCluster].phiCoordinate)) ;

    }
  /*
    GPU_DEVICE inline int** PrimaryVertexContext::getIndexTables()
    {
      return mIndexTables;
    }
  */
    GPU_DEVICE inline TrackletStruct** PrimaryVertexContext::getTracklets()
    {
      return mTracklets;
    }

    GPU_DEVICE inline int** PrimaryVertexContext::getTrackletsLookupTable()
    {
      return mTrackletsLookupTable;
    }

    GPU_DEVICE inline int** PrimaryVertexContext::getTrackletsPerClusterTable()
    {
      return mTrackletsPerClusterTable;
    }
  #endif
}
}
}
}
