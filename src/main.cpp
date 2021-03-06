/***********************************************************************************
    Copyright 2015  Hung-Yi Pu, Kiyun Yun, Ziri Younsi, Sunk-Jin Yoon
                        Odyssey  version 1.0   (released  2015)
    This file is part of Odyssey source code. Odyssey is a public, GPU-based code 
    for General Relativistic Radiative Transfer (GRRT), following the 
    ray-tracing algorithm presented in 
    Fuerst, S. V., & Wu, K. 2007, A&A, 474, 55, 
    and the radiative transfer formulation described in 
    Younsi, Z., Wu, K., & Fuerst, S. V. 2012, A&A, 545, A13
    
    Odyssey is distributed freely under the GNU general public license. 
    You can redistribute it and/or modify it under the terms of the License
        http://www.gnu.org/licenses/gpl.txt
    The current distribution website is:
	https://github.com/hungyipu/Odyssey/ 
	
    We ask that users of Odyssey cite the following paper in their subsequent scientific 
    literature and publications which result from the use of any part of Odyssey:
    "Odyssey: A PUBLIC GPU-BASED CODE FOR GENERAL-RELATIVISTIC RADIATIVE TRANSFER 
    IN KERR SPACE-TIME" 
    by Hung-Yi Pu, Kiyun Yun, Ziri Younsi, and Suk-Jin Yoon (2016 ApJ 820, 105) 
	
***********************************************************************************/
#pragma once

#include <cuda.h>
#include <fstream>
#include <iostream>
#include "task1.h"
#include "task2.h"


using namespace std;
using namespace Task1; 
using namespace Task2; 

namespace OdysseyTasks
{
	
	void task1()
	{
		printf("task1: ...Reproduce Figure 5.2 of Agol(1997) ...\n");
		mission1 mission;

		double  VariablesIn[VarINNUM];
		double* Results;
		int	    ImaDimX, ImaDimY;                	//=number of grids; the coordinate of each grid is given by (GridIdxX,GridIdY)
		int		GridDimX, GridDimY;		//=number of blocks; the coordinate of each block is given by (blockIdx.x ,blockIdx.y )
		int     BlockDimX, BlockDimY;			//=number of threads; the coordinate of each thread is given by (threadIdx.x,threadIdx.y)

		//assign parameters
		A			    = 0.;		// black hole spin
		INCLINATION     = acos(0.25)/PI*180.;	 	// inclination angle in unit of degree		                
		SIZE			= 512;			// pixels of the image

		printf("image size = %.0f  x  %0.f  pixels\n",SIZE,SIZE);


		//assign CUDA congfiguration
		BlockDimX = 100;
		BlockDimY = 1;
		GridDimX  = 1;
		GridDimY  = 50;
		mission.setDims(GridDimX, GridDimY, BlockDimX, BlockDimY);


		//allocate memory on device for input and output
		mission.PRE(VariablesIn);
		 

		//compute number of grids, to cover the whole image plane
		ImaDimX = (int)ceil((double)SIZE / (BlockDimX * GridDimX));
                ImaDimY = (int)ceil((double)SIZE / (BlockDimY * GridDimY));


		//perform the for-loop for GRRT 
		for(int GridIdxY = 0; GridIdxY < ImaDimY; GridIdxY++){
        	for(int GridIdxX = 0; GridIdxX < ImaDimX; GridIdxX++){                			
				mission.GPUCompute(GridIdxX, GridIdxY);
        	}
			printf(" finish %2.0f %%\n",((GridIdxY+1.)*1.0)/(ImaDimY*1.0)*100.);
		}

		
		Results = new double[(int)SIZE * (int)SIZE * 3];

		//copy memory form device to host and free CUDA memory
		mission.AFTER(Results);


		//save result to output
		FILE *fp1;
		fp1=fopen("Output_task1.txt","w");  
		fprintf(fp1,"###Computed by Odyssey\n");
		fprintf(fp1,"###output data:(alpha,  beta,  redshift)\n");

		for(int j = 0; j < (int)SIZE; j++)
		for(int i = 0; i < (int)SIZE; i++)
		{
			fprintf(fp1, "%f\t", (float)Results[3 * ((int)SIZE * j + i) + 0]);
			fprintf(fp1, "%f\t", (float)Results[3 * ((int)SIZE * j + i) + 1]);
			fprintf(fp1, "%f\n", (float)Results[3 * ((int)SIZE * j + i) + 2]);
		}
		fclose(fp1);


	}
	
	void task2()
	{
		printf("task2: ...compute thermal syn image of a Keplerian rotating shell...\n");
		mission2 mission;

		double  VariablesIn[VarINNUM];
		double* Results;
		int	    ImaDimX, ImaDimY;                	//=number of grids; the coordinate of each grid is given by (GridIdxX,GridIdY)
		int		GridDimX, GridDimY;		//=number of blocks; the coordinate of each block is given by (blockIdx.x ,blockIdx.y )
		int     BlockDimX, BlockDimY;			//=number of threads; the coordinate of each thread is given by (threadIdx.x,threadIdx.y)

		//assign parameters
		A			    = 0.;		// black hole spin
		INCLINATION     = 45.;	                   	// inclination angle in unit of degree		                
		SIZE			= 128;			// pixels of the image
		freq_obs        = 340e9;                       	// observed frequency

		printf("image size = %.0f  x  %0.f  pixels\n",SIZE,SIZE);


		//assign CUDA congfigurations
		BlockDimX = 100;
		BlockDimY = 1;
		GridDimX  = 1;
		GridDimY  = 50;
		mission.setDims(GridDimX, GridDimY, BlockDimX, BlockDimY);


		//allocate memory on device for input and output
		mission.PRE(VariablesIn);
		 

		//compute number of grides, to cover the whole image plane
		ImaDimX = (int)ceil((double)SIZE / (BlockDimX * GridDimX));
        ImaDimY = (int)ceil((double)SIZE / (BlockDimY * GridDimY));


		//perform the for-loop for GRRT 
		for(int GridIdxY = 0; GridIdxY < ImaDimY; GridIdxY++){
        	for(int GridIdxX = 0; GridIdxX < ImaDimX; GridIdxX++){                			
				mission.GPUCompute(GridIdxX, GridIdxY);
        	}
			printf(" finish %2.0f %%\n",((GridIdxY+1.)*1.0)/(ImaDimY*1.0)*100.);
		}

		
		Results = new double[(int)SIZE * (int)SIZE * 3];

		//copy memory form device to host and free CUDA memory
		mission.AFTER(Results);




		//save result to output
		FILE *fp1;
		fp1=fopen("Output_task2.txt","w");  
		fprintf(fp1,"###Computed by Odyssey\n");
		fprintf(fp1,"###output data:(alpha,  beta, Luminosity (erg/sec))\n");

		for(int j = 0; j < (int)SIZE; j++)
		for(int i = 0; i < (int)SIZE; i++)
		{
			fprintf(fp1, "%f\t", (float)Results[3 * ((int)SIZE * j + i) + 0]);
			fprintf(fp1, "%f\t", (float)Results[3 * ((int)SIZE * j + i) + 1]);
			fprintf(fp1, "%f\n", (float)Results[3 * ((int)SIZE * j + i) + 2]);
		}
		fclose(fp1);


	}
}

void main()
{
	cudaSetDevice(0);	
	OdysseyTasks::task1();
	//OdysseyTasks::task2();
}
