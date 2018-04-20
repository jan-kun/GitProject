//#include "stdafx.h"
#include "stdio.h"
#include "math.h"


#include "RobConvert.h"
#include"string.h"

static void GetRobotRPY(double xAis[6],mat  *pM);
static void SetRobotRPY(double xAis[6],mat  *pM);
static void Matrix2Quat(mat  *pM, double q[]);
static void Quat2Matrix(mat  *pM, double q[]);
static void ReverMatrix(mat  *pM,mat  *pR);
static void MatrixMult(mat *pR,mat *pT0,mat *pT1);
static void ReverseXY(mat *pR);
static void VerseCalc(double xAis[6],double aLen[6],mat *pCrnP);
static  int GetToolSize(int iPnt[9][6],double Len[6],int num,double *pTool,double *pErr);
static void  GetSubMatrix(mat *pCrnP,double RotM[24][3],double p[24],int num);
static int ReverMatrix_3(rot *Rot);
static void GetMatrix_XZ(double aPoint[3][3],mat *pMat,mat oMat,double aTool[7]);
static void MulVector(double res[3],double a[3],double b[3]);
static void GetPoint(mat *pMat,double tool[3],double aPoint[3]);

/*--------------------------------------------------------------------------
fun     ：三点法计算用户坐标数据，用四元素表示,
ret     ：计算正确返回0，失败返回1两点重合 2三点共线
iX1[3]   ：x上一点位置
iX2[3]   ：x轴方向位置
iY[3]    : y轴上位置
---------------------------------------------------------------------------*/
extern int GetWobjdata(int iX1[3],int iX2[3],int iY[3],double quat[7])
{
    double x1[3],x2[3],y[3],o[3],k;
    double d12[3],d13[3];
    double Eula[6];
    mat uMat;
    for(int i=0;i<3;i++)
    {
        x1[i]=iX1[i]/100000;
        x2[i]=iX2[i]/100000;
        y [i]=iY[i] /100000;
    }
    /*  figure out original position  */
    for(int i=0;i<3;i++)
    {
        d12[i]=x2[i]-x1[i];
        d13[i]=x1[i]-y[i];
    }
    /*2 point repeat*/
    if(	(  d12[0]*d12[0]+d12[1]*d12[1]+d12[2]*d12[2]==0)
        ||(d13[0]*d13[0]+d13[1]*d13[1]+d13[2]*d13[2]==0)
        ||(d12[0]==d13[0]&&d12[1]==d13[1]&&d12[2]==d13[2]))                    return 1;
    /*three point in a line */
    if((d12[0]/d13[0] == d12[1]/d13[1]) && (d12[0]/d13[0] == d12[2]/d13[2]))   return 2;

    k=(d13[0]*d12[0]+d13[1]*d12[1]+d13[2]*d12[2])/(d12[0]*d12[0]+d12[1]*d12[1]+d12[2]*d12[2]);
    for(int i=0;i<3;i++)   o[i]=x1[i]-k*d12[i];

    /*X coordinate          */
    k=sqrt(d12[0]*d12[0]+d12[1]*d12[1]+d12[2]*d12[2]);
    uMat.mA[0][0]=d12[0]/k;
    uMat.mA[1][0]=d12[1]/k;
    uMat.mA[2][0]=d12[2]/k;
    uMat.mA[3][0]=0;

    /*Y coordinate          */
    k=sqrt((y[0]-o[0])*(y[0]-o[0])+(y[1]-o[1])*(y[1]-o[1])+(y[2]-o[2])*(y[2]-o[2]));
    uMat.mA[0][1]=(y[0]-o[0])/k;
    uMat.mA[1][1]=(y[1]-o[1])/k;
    uMat.mA[2][1]=(y[2]-o[2])/k;
    uMat.mA[3][1]=0;

    /*Z coordinate     Z=XxY     */

    uMat.mA[0][2]=uMat.mA[1][0]*uMat.mA[2][1]-uMat.mA[2][0]*uMat.mA[1][1];
    uMat.mA[1][2]=uMat.mA[2][0]*uMat.mA[0][1]-uMat.mA[0][0]*uMat.mA[2][1];
    uMat.mA[2][2]=uMat.mA[0][0]*uMat.mA[1][1]-uMat.mA[1][0]*uMat.mA[0][1];
    uMat.mA[3][2]=0;

    /*origin    */
    uMat.mA[0][3]=o[0];
    uMat.mA[1][3]=o[1];
    uMat.mA[2][3]=o[2];
    uMat.mA[3][3]=1;

    Matrix2Quat(&uMat,quat);
    return 0;
}

/*--------------------------------------------------------------------------
fun     ：把用户坐标系位置、姿态转化为世界坐标系位置、姿态
uQ[7]   ：用户坐标系下位置和姿态
wQ[7]   ：世界坐标系下位置和姿态
qCoor[7]: 用户坐标系参数
---------------------------------------------------------------------------*/
extern void User2World(double uQ[7],double wQ[7],double qCoor[7])
{

    mat mUser,mCoor,mRes;
    Quat2Matrix(&mUser,uQ);
    /*XY方向取反        */
    ReverseXY(&mUser);
    Quat2Matrix(&mCoor,qCoor);
    MatrixMult(&mRes,&mCoor,&mUser);
    /*XY方向取反        */
    ReverseXY(&mRes);
    Matrix2Quat(&mRes,wQ);
}
/*--------------------------------------------------------------------------
fun     ：把世界坐标系位置、姿态转化为用户坐标系位置、姿态
uQ[7]   ：用户坐标系下位置和姿态
wQ[7]   ：世界坐标系下位置和姿态
qCoor[7]: 用户坐标系参数
---------------------------------------------------------------------------*/
extern void World2User(double uQ[7],double wQ[7],double qCoor[7])
{
    mat mWorld,mCoor,k,mRes;
    Quat2Matrix(&mWorld,wQ);
    /*XY方向取反        */
    ReverseXY(&mWorld);
    Quat2Matrix(&mCoor,qCoor);
    ReverMatrix(&mCoor,&k);
    MatrixMult(&mRes,&k,&mWorld);
    /*XY方向取反        */
    ReverseXY(&mRes);
    Matrix2Quat(&mRes,uQ);
}

/*--------------------------------------------------------------------------
fun： 系统读出的Eula角直接转换成四元素
Eula：系统读出的XYZWRP放大k倍
q[7]：依次XYZq1q2q3q4无放大
k:    系统传出数据默认放大100000倍
---------------------------------------------------------------------------*/
extern void  Eula2Quat(i32 Eula[6],double q[7],int k)
{
    double a[6];
    mat  Mat;
    for(char i=0;i<6;i++)   a[i]=(double)Eula[i]/k;
    SetRobotRPY(a,&Mat);

    /*XY方向取反        */
    for(char i=0;i<2;i++)
    {
        for(char j=0;j<3;j++)
        {
            Mat.mA[j][i]=-Mat.mA[j][i];
        }
    }
    Matrix2Quat(&Mat,q);

}
/*--------------------------------------------------------------------------
fun： 根据四元素转化成Eula角直接放大k倍，直接发送到系统中
q[7]：依次XYZq1q2q3q4无放大
Eula：Eula角直接放大k倍后数据，直接发送数据
K：   放大倍数,点数据是100000倍
---------------------------------------------------------------------------*/
extern void Quat2Eula(int Eula[6],double q[7],int k)
{
    double a[6];
    mat  Mat;
    Quat2Matrix(&Mat,q);
    /*XY方向取反        */
    for(char i=0;i<2;i++)
    {
        for(char j=0;j<3;j++)
        {
            Mat.mA[j][i]=-Mat.mA[j][i];
        }
    }
    GetRobotRPY(a,&Mat);
    for(char i=0;i<6;i++)   Eula[i]=(int)(a[i]*k);

}

/*--------------------------------------------------------------------------
fun： 根据四元素转化成Eula角直接放大k倍，直接发送到系统中
q[7]：依次XYZq1q2q3q4无放大
Eula：Eula角直接放大k倍后数据，直接发送数据
K：   放大倍数,工具、用户坐标数据放大1000倍
---------------------------------------------------------------------------*/
extern void Coor2Eula(int Eula[6],double q[7],int k)
{
    double a[6];
    mat  Mat;
    Quat2Matrix(&Mat,q);

    GetRobotRPY(a,&Mat);
    for(char i=0;i<6;i++)   Eula[i]=(int)(a[i]*k);

}

/*------------------------------------------------------------------------------
局部函数，调用不管
-------------------------------------------------------------------------------*/
static void ReverMatrix(mat  *pM,mat  *pR)
{
    for(char i=0;i<3;i++)
    {
        for(char j=0;j<3;j++)
        {
            pR->mA[i][j]=pM->mA[j][i];
        }
    }
    pR->mA[3][0]=0;
    pR->mA[3][1]=0;
    pR->mA[3][2]=0;
    pR->mA[3][3]=1;
    for(char i=0;i<3;i++)
        pR->mA[i][3]=-pR->mA[i][0]*pM->mA[0][3]
                     -pR->mA[i][1]*pM->mA[1][3]
                     -pR->mA[i][2]*pM->mA[2][3];
}



static void Quat2Matrix(mat  *pM, double q[7])
{
    double wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
    double rlength2;
    /*与ABB方向相反的向量     */
    q[4]=-q[4];
    q[5]=-q[5];
    q[6]=-q[6];

    rlength2 = 2.0/(q[3]*q[3]+q[4]*q[4]+q[5]*q[5]+q[6]*q[6]);

    x2 = rlength2*q[4];
    y2 = rlength2*q[5];
    z2 = rlength2*q[6];

    xx = q[4] * x2;
    xy = q[4] * y2;
    xz = q[4] * z2;

    yy = q[5] * y2;
    yz = q[5] * z2;
    zz = q[6] * z2;

    wx = q[3] * x2;
    wy = q[3] * y2;
    wz = q[3] * z2;

    pM->mA[0][0] = -yy - zz+1;
    pM->mA[1][0] =  xy - wz ;
    pM->mA[2][0] = +xz + wy;

    pM->mA[0][1] =  xy + wz;
    pM->mA[1][1] = -xx - zz +1;
    pM->mA[2][1] =  yz - wx ;

    pM->mA[0][2] = xz - wy;
    pM->mA[1][2] = yz + wx;
    pM->mA[2][2] = 1.0 - xx - yy;

    pM->mA[0][3]=q[0];
    pM->mA[1][3]=q[1];
    pM->mA[2][3]=q[2];
    pM->mA[3][3]=1;

    pM->mA[3][0]=0;
    pM->mA[3][1]=0;
    pM->mA[3][2]=0;

}
static void Matrix2Quat(mat  *pM, double q[7])
{

    double s, tq[4];
    char    i, j;

    // Use tq to store the largest trace
    tq[0] = 1 + pM->mA[0][0]+pM->mA[1][1]+pM->mA[2][2];
    tq[1] = 1 + pM->mA[0][0]-pM->mA[1][1]-pM->mA[2][2];
    tq[2] = 1 - pM->mA[0][0]+pM->mA[1][1]-pM->mA[2][2];
    tq[3] = 1 - pM->mA[0][0]-pM->mA[1][1]+pM->mA[2][2];

    // Find the maximum (could also use stacked if's later)
    j = 0;
    for(i=1;i<4;i++) j = (tq[i]>tq[j])? i : j;

    // check the diagonal
    if (j==0)
    {
        /* perform instant calculation */
        q[3] = tq[0];
        q[4] = pM->mA[1][2]-pM->mA[2][1];
        q[5] = pM->mA[2][0]-pM->mA[0][2];
        q[6] = pM->mA[0][1]-pM->mA[1][0];
    }
    else if (j==1)
    {
        q[3] = pM->mA[1][2]-pM->mA[2][1];
        q[4] = tq[1];
        q[5] = pM->mA[0][1]+pM->mA[1][0];
        q[6] = pM->mA[2][0]+pM->mA[0][2];
    }
    else if (j==2)
    {
        q[3] = pM->mA[2][0]-pM->mA[0][2];
        q[4] = pM->mA[0][1]+pM->mA[1][0];
        q[5] = tq[2];
        q[6] = pM->mA[1][2]+pM->mA[2][1];
    }
    else /* if (j==3) */
    {
        q[3] = pM->mA[0][1]-pM->mA[1][0];
        q[4] = pM->mA[2][0]+pM->mA[0][2];
        q[5] = pM->mA[1][2]+pM->mA[2][1];
        q[6] = tq[3];
    }

    s = sqrt(0.25/tq[j]);
    q[3] *=  s;
    q[4] *= -s;
    q[5] *= -s;
    q[6] *= -s;
    q[0] =pM->mA[0][3];
    q[1] =pM->mA[1][3];
    q[2] =pM->mA[2][3];
}

/*********************************************************************************
FUNCTION :根据变换矩阵计算姿态(回转角，俯仰角，偏转角)
PARAMETER:
********************************************************************************/
static void GetRobotRPY(double xAis[6],mat  *pM)
{
    xAis[0]=pM->mA[0][3];
    xAis[1]=pM->mA[1][3];
    xAis[2]=pM->mA[2][3];
    if(pM->mA[2][0]==1)
    {
        xAis[4]=-PI/2;
        xAis[5]=0;
        xAis[3]=-atan2(pM->mA[0][1],pM->mA[1][1]);
    }
    else if(pM->mA[2][0]==-1)
    {
        xAis[4]=PI/2;
        xAis[5]=0;
        xAis[3]=atan2(pM->mA[0][1],pM->mA[1][1]);
    }
    else
    {
        //RY
        xAis[4]=atan2(-pM->mA[2][0],sqrt(pM->mA[0][0]*pM->mA[0][0]+pM->mA[1][0]*pM->mA[1][0]));

        //RZ
        xAis[5]=atan2(pM->mA[1][0]/cos(xAis[4]),pM->mA[0][0]/cos(xAis[4]));

        //RX
        xAis[3]=atan2(pM->mA[2][1]/cos(xAis[4]),pM->mA[2][2]/cos(xAis[4]));

    }

}
/*********************************************************************************
FUNCTION :根据姿态计算变换矩阵
PARAMETER:
********************************************************************************/
static  void SetRobotRPY(double xAis[6],mat  *pM)
{
    double c1,s1,c2,s2,c3,s3;
    c1=cos(xAis[3]);  s1=sin(xAis[3]);
    c2=cos(xAis[4]);  s2=sin(xAis[4]);
    c3=cos(xAis[5]);  s3=sin(xAis[5]);

    pM->mA[0][0]=c3*c2;
    pM->mA[1][0]=s3*c2;
    pM->mA[2][0]=-s2;

    pM->mA[0][1]=c3*s2*s1-s3*c1;
    pM->mA[1][1]=s3*s2*s1+c3*c1;
    pM->mA[2][1]=c2*s1;

    pM->mA[0][2]=c3*s2*c1+s3*s1;
    pM->mA[1][2]=s3*s2*c1-c3*s1;
    pM->mA[2][2]=c2*c1;

    pM->mA[0][3]=xAis[0];
    pM->mA[1][3]=xAis[1];
    pM->mA[2][3]=xAis[2];

    pM->mA[3][0]=0;
    pM->mA[3][1]=0;
    pM->mA[3][2]=0;
    pM->mA[3][3]=1;

}
static void MatrixMult(mat *pR,mat *pT0,mat *pT1)
{
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            pR->mA[i][j] = pT0->mA[i][0]*pT1->mA[0][j] +
                           pT0->mA[i][1]*pT1->mA[1][j] +
                           pT0->mA[i][2]*pT1->mA[2][j] +
                           pT0->mA[i][3]*pT1->mA[3][j];
        }
    }
}
static void ReverseXY(mat *pR)
{
    for(char i=0;i<3;i++)
    {
        for(char j=0;j<2;j++)
        {
            pR->mA[i][j]=-pR->mA[i][j];
        }
    }
}

/*--------------------------------------------------------------------------
fun      ：计算工具数据
ret      ：计算正确返回0
iWay     :点数和方式
iPnt     :关节位置
size     :机器人尺寸
---------------------------------------------------------------------------*/
extern int GetTooldata(int iPnt[11][6],int size[6],int iWay,double aTool[7],double aErr[3])
{
    int ret=0;
    int num=iWay & 0xf;
    int Type=iWay & 0xf0;
    double len[6];

    double aXis[6];
    double  aPoint[3][3];
    mat     aMrt[3];


    for(int i=0;i<6;i++)  len[i]=(double)size[i]/1000;
    ret=GetToolSize(iPnt,len,num,aTool,aErr);
    if(ret==0)
    {
        if(Type==0)    return 0;
        else if(Type==0x10)
        {
            for(int i=0;i<2;i++)
            {
                for(int k=0;k<6;k++)		aXis[k]=(double)iPnt[num+i-1][k]/100000;
                VerseCalc(aXis,len,&aMrt[i]);
                GetPoint(aMrt+i,aTool,aPoint[i]);
            }
            aPoint[2][0]=aPoint[1][0];
            aPoint[2][1]=aPoint[1][1];
            aPoint[2][2]=aPoint[1][2];

            aPoint[1][0]=0;
            aPoint[1][1]=0;
            aPoint[1][2]=0;
            GetMatrix_XZ(aPoint,aMrt+1,aMrt[0],aTool);
            Matrix2Quat(aMrt+1,aTool);
        }
        else
        {
            for(int i=0;i<3;i++)
            {
                for(int k=0;k<6;k++)		aXis[k]=(double)iPnt[num+i-1][k]/100000;
                VerseCalc(aXis,len,&aMrt[i]);
                GetPoint(aMrt+i,aTool,aPoint[i]);
            }
            GetMatrix_XZ(aPoint,aMrt+1,aMrt[0],aTool);
            Matrix2Quat(aMrt+1,aTool);
        }
    }
    // error
    else
    {
        return ret;
    }

    return 0;
}




/*--------------------------------------------------------------------------*/
static  int GetToolSize(int iPnt[9][6],double Len[6],int num,double *pTool,double *pErr)
{
    mat  aMrt[9];
    rot  aVer;
    double aXis[6];

    double RotM[24][3],p[24];
    double RotH[3][24],RotA[3][24];

    memset(RotM,0,576);
    memset(RotA,0,576);
    memset(p,0,192);
    memset(&aVer,0,sizeof(rot));



    for(int i=0;i<num;i++)
    {
        for(int k=0;k<6;k++)		aXis[k]=(double)iPnt[i][k]/100000;
        VerseCalc(aXis,Len,&aMrt[i]);
    }

  GetSubMatrix(aMrt,RotM,p,num);

    for(int m=0;m<3;m++)
    {
        for(int n=0;n<24;n++)
        {
            RotH[m][n]=RotM[n][m];
        }
    }
    // A^H*A
    for(int m=0;m<3;m++)
    {
        for(int n=0;n<3;n++)
        {
            for(int i=0;i<24;i++)
            {
                aVer.aXis[m][n]=aVer.aXis[m][n]+RotH[m][i]*RotM[i][n];
            }
        }
    }
    if(ReverMatrix_3(&aVer))
    {
        return 1;
    }


    for(int m=0;m<3;m++)
    {
        for(int n=0;n<24;n++)
        {
            for(int i=0;i<3;i++)
            {
                RotA[m][n]=RotA[m][n]+aVer.aXis[m][i]*RotH[i][n];
            }
        }
    }

    for(int i=0;i<24;i++)
    {
        *pTool    =*pTool     + RotA[0][i]*p[i];
        *(pTool+1)=*(pTool+1) + RotA[1][i]*p[i];
        *(pTool+2)=*(pTool+2) + RotA[2][i]*p[i];
    }

    return 0;
}


/*--------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------*/
static int ReverMatrix_3(rot *Rot)
{
    double det,M[3][3];
    float j;
    det=Rot->aXis[0][0]*Rot->aXis[1][1]*Rot->aXis[2][2]+
        Rot->aXis[0][1]*Rot->aXis[1][2]*Rot->aXis[2][0]+
        Rot->aXis[0][2]*Rot->aXis[1][0]*Rot->aXis[2][1]-
        Rot->aXis[0][0]*Rot->aXis[1][2]*Rot->aXis[2][1]-
        Rot->aXis[0][1]*Rot->aXis[1][0]*Rot->aXis[2][2]-
        Rot->aXis[0][2]*Rot->aXis[1][1]*Rot->aXis[2][0];
    j=(float) det;
    if(j==0)  return 1;
    else
    {
      M[0][0]=Rot->aXis[1][1]*Rot->aXis[2][2]-Rot->aXis[1][2]*Rot->aXis[2][1];
      M[0][1]=Rot->aXis[1][0]*Rot->aXis[2][2]-Rot->aXis[1][2]*Rot->aXis[2][0];
      M[0][2]=Rot->aXis[1][0]*Rot->aXis[2][1]-Rot->aXis[1][1]*Rot->aXis[2][0];


      M[1][0]=Rot->aXis[0][1]*Rot->aXis[2][2]-Rot->aXis[0][2]*Rot->aXis[2][1];
      M[1][1]=Rot->aXis[0][0]*Rot->aXis[2][2]-Rot->aXis[2][0]*Rot->aXis[0][2];
      M[1][2]=Rot->aXis[0][0]*Rot->aXis[2][1]-Rot->aXis[0][1]*Rot->aXis[2][0];

      M[2][0]=Rot->aXis[0][1]*Rot->aXis[1][2]-Rot->aXis[0][2]*Rot->aXis[1][1];
      M[2][1]=Rot->aXis[0][0]*Rot->aXis[1][2]-Rot->aXis[0][2]*Rot->aXis[1][0];
      M[2][2]=Rot->aXis[0][0]*Rot->aXis[1][1]-Rot->aXis[0][1]*Rot->aXis[1][0];

      M[0][1] = -M[0][1];
      M[1][0] = -M[1][0];
      M[1][2] = -M[1][2];
      M[2][1] = -M[2][1];
      for(char  i=0;i<3;i++)
      {
         for(char  m=0;m<3;m++)   Rot->aXis[i][m]=M[m][i]/det;
      }
      return 0;
    }

}
static void  GetSubMatrix(mat *pCrnP,double RotM[24][3],double p[24],int num)
{
    for(int i=1;i<num;i++)
    {
        for(int m=0;m<3;m++)
        {
            for(int n=0;n<3;n++)
            {
                RotM[m+3*(i-1)][n]=(pCrnP+i)->mA[m][n] - pCrnP->mA[m][n];
            }
            p[m+3*(i-1)]=pCrnP->mA[m][3] -(pCrnP+i)->mA[m][3] ;
        }
    }
}


static void VerseCalc(double xAis[6],double aLen[6],mat *pCrnP)
{
  double c23,s23,tn1,tn2,tn3,tn4;
  double s1,s2,s4,s5,s6,c1,c2,c4,c5,c6;

  s23  = sin(xAis[1]-xAis[2]);
  c23  = cos(xAis[1]-xAis[2]);
  s1=sin(xAis[0]);	c1=cos(xAis[0]);
  s2=sin(xAis[1]);	c2=cos(xAis[1]);
//	s3=sin(xAis[2]);	c3=cos(xAis[2]);
  s4=sin(xAis[3]);	c4=cos(xAis[3]);
  s5=sin(xAis[4]);	c5=cos(xAis[4]);
  s6=sin(xAis[5]);	c6=cos(xAis[5]);

  tn1 =c4*c5*c6+s4*s6;
  tn2 =s4*c5*c6-c4*s6;
  pCrnP->mA[0][0] =	c1*s23*tn1-c1*c23*s5*c6-s1*tn2;
  pCrnP->mA[1][0] =	s1*s23*tn1-s1*c23*s5*c6+c1*tn2;;
  pCrnP->mA[2][0] =	c23*(c4*c5*c6+s4*s6)+s23*s5*c6;
  pCrnP->mA[3][0] =	0;

  tn1 =-c4*c5*s6+s4*c6;
  tn2 =s4*c5*s6+c4*c6;

  pCrnP->mA[0][1] =	c1*s23*tn1+c1*c23*s5*s6+s1*tn2;
  pCrnP->mA[1][1] =	s1*s23*tn1+s1*c23*s5*s6-c1*tn2;
  pCrnP->mA[2][1] =	c23*tn1-s23*s5*s6;
  pCrnP->mA[3][1] =	0;

  pCrnP->mA[0][2] =	c1*(s23*c4*s5+c23*c5)-s1*s4*s5;
  pCrnP->mA[1][2] =	s1*(s23*c4*s5+c23*c5)+c1*s4*s5;
  pCrnP->mA[2][2]=	c23*c4*s5-s23*c5;
  pCrnP->mA[3][2] =	0;

  tn1=c4*s5*aLen[5]+aLen[3];
  tn2=c5*aLen[5]+aLen[4];
  tn3=s23*tn1+c23*tn2+s2*aLen[2]+aLen[1];
  tn4=s4*s5*aLen[5];
  pCrnP->mA[0][3] =	c1*tn3-s1*tn4;
  pCrnP->mA[1][3] =	s1*tn3+c1*tn4;
  pCrnP->mA[2][3] =	c23*tn1-s23*tn2+aLen[2]*c2;
  pCrnP->mA[3][3] =	1;
}


static void GetMatrix_XZ(double aPoint[3][3],mat *pMat,mat oMat,double aTool[7])
{
    double n[3],o[3],a[3];
    double VX[3],VZ[3],k1,k2,k3;
    rot    Orot,ROrot;
    for(int i=0;i<3;i++)
    {
        VX[i]=aPoint[1][i]-aPoint[0][i];
        VZ[i]=aPoint[2][i]-aPoint[0][i];
    }




    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            Orot.aXis[i][j] =oMat.mA[i][j];
            ROrot.aXis[i][j]=Orot.aXis[j][i];
        }
        n[i]=ROrot.aXis[i][0]*VX[0]+ROrot.aXis[i][1]*VX[1]+ROrot.aXis[i][2]*VX[2];
        a[i]=ROrot.aXis[i][0]*VZ[0]+ROrot.aXis[i][1]*VZ[1]+ROrot.aXis[i][2]*VZ[2];
    }


    //Y=Z*X
    MulVector(o,a,n);
    //X=Y*Z
    MulVector(n,o,a);

    k1=sqrt(n[0]*n[0]+n[1]*n[1]+n[2]*n[2]);
    k2=sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
    k3=sqrt(o[0]*o[0]+o[1]*o[1]+o[2]*o[2]);
    for(int i=0;i<3;i++)
    {
        n[i]=n[i]/k1;
        a[i]=a[i]/k2;
        o[i]=o[i]/k3;
    }


    for(int i=0;i<3;i++)
    {
        pMat->mA[i][0]=n[i];
        pMat->mA[i][1]=o[i];
        pMat->mA[i][2]=a[i];
        pMat->mA[3][i]=0;
    }
    pMat->mA[0][3]=aTool[0];
    pMat->mA[1][3]=aTool[1];
    pMat->mA[2][3]=aTool[2];
    pMat->mA[3][3]=1;
}


//a=(X1,Y1,Z1),b=(X2,Y2,Z2),
//a×b=（Y1Z2-Y2Z1,Z1X2-Z2X1,X1Y2-X2Y1）
static void MulVector(double res[3],double a[3],double b[3])
{
    res[0]=a[1]*b[2]-b[1]*a[2];
    res[1]=a[2]*b[0]-b[2]*a[0];
    res[2]=a[0]*b[1]-b[0]*a[1];
}

static void GetPoint(mat *pMat,double tool[3],double aPoint[3])
{
    for(int i=0;i<3;i++)   aPoint[i]=pMat->mA[i][3]+pMat->mA[i][0]*tool[0]+pMat->mA[i][1]*tool[1]+pMat->mA[i][2]*tool[2];
}
