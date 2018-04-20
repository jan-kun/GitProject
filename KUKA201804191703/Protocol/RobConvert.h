#ifndef _convert_H
#define _convert_H

#include"Protocol/linuxtypes.h"

#define  PI   3.1415926535898

/*姿态矩阵                    */
typedef struct
{
    double            mA[4][4];
}mat;

typedef struct
{
    double            aXis[3][3];
}rot;

extern void Quat2Eula(int Eula[6],double q[7],int k);
extern void Coor2Eula(int Eula[6],double q[7],int k);

extern void Eula2Quat(i32 Eula[], double q[7], int k);
extern void User2World(double uQ[7],double wQ[7],double qCoor[7]);
extern void World2User(double uQ[7],double wQ[7],double qCoor[7]);
extern int GetWobjdata(int iX1[3],int iX2[3],int iY[3],double quat[7]);
extern int GetTooldata(int iPnt[11][6],int size[6],int iWay,double aTool[7],double aErr[3]);


#endif
