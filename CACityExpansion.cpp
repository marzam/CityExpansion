   /*
 *  CACityExpansion.cpp
 *  TCA
 *
 *  Created by Marcelo Zamith on 3/15/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include <CACityExpansion.hpp>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <ctime>
#include <climits>
#include <cstring>
#include <cmath>
using namespace std;

/*
 *
 */
CACityExpansion::CACityExpansion():
mRunning(false),
mInit(false),
mState0(NULL),
mState1(NULL),
mWidth(0),
mHeight(0),
mAttrList(NULL),
mRadius(1)
{
  cout << "\nStarting application" << endl;
  srand (time(NULL));
}

/*
 *
 */
CACityExpansion::~CACityExpansion()
{ cout << "\nFinalizing application" << endl;  }




/*
 * Clear memories
 */
//void CACityExpansion::clear(void) { mRules->getGrid()->allocGrid(mParam.cellX, mRules->getParam()->cellY); };

void CACityExpansion::help(void){
    cout << "HELP" << endl;
}


/**
 *  inicialize
 *  @param fileName: config file name
 */
void CACityExpansion::init(void){
    cout << "Init only flags" << endl;
    mInit = true;
    mRunning = false;

}

/**
 * Finalize and dealloc all variables.
 * It also saves log file
 */
void CACityExpansion::finalize(void){
  if (mState0 != NULL) free(mState0);
  if (mState1 != NULL) free(mState1);
  mState0 = mState1 = NULL;

  if (mAttrList != NULL){
    for (int k = 0; k < mWidth *   mHeight; k++){
      stAttribList *ptrA = reinterpret_cast<stAttribList*>(mAttrList[k]);
      stAttribList *auxA = ptrA;
      while (auxA != NULL){

        if (auxA->curr.weights != NULL)
          free(auxA->curr.weights);
        auxA->curr.weights = NULL;
        if (auxA->curr.values != NULL)
         free(auxA->curr.values);
        auxA->curr.values = NULL;


        ptrA = auxA->next;
        free(auxA);
        auxA = NULL;
        auxA = ptrA;
      }
    }

    free(mAttrList);
    mAttrList = NULL;

  }
  cout << "finalize " << endl;


};


int  CACityExpansion::getState(int i, int j)  { return   mState0[mWidth * j + i]; }
void CACityExpansion::setState(int i, int j, uint8_t s){ mState0[mWidth * j + i]  = s; }

/*
 * Running the simulation
 */
void CACityExpansion::update(void){
  if (!mRunning)
    return;

  int r = mRadius;
  float dist  = 0.0f,
        cell,
        N,
        A = 6.0f;
  for (int j = 1; j < mHeight - 1; j++)
      for (int i = 1; i < mWidth - 1; i++){
          N = 0.0f;
          int p = (j * mWidth) + i;
          for (int rj = -r; rj <= r; rj++){
              int pj = j - rj;
              for (int ri = -r; ri <= r; ri++){
                  int pi = i + ri;
                  //if ((pi >= 0) && (pi < mWidth) && (pj >= 0) && (pj < mHeight))
                  {
                      p = (pj * mWidth) + pi;
                      cell = 0.0f;
                      if (!((rj == 0) && (ri == 0))){
                          cell = static_cast<float>(mState0[p]);
                          dist = distance(0, ri, 0, rj);
                          N += (cell / dist);
                      }
                  }//end-if ((pi >= 0) && (pi < mCellX) && (pj >= 0) && (pj < mCellY))
                     // sum++;
              }//end-for (int ri = -r; ri <= r; ri++){
          }//end-for (int rj = -r; rj <= r; rj++){

          float aux = N / A;
          if (aux > 100.0f){
            cerr << "A: " << A << endl;
            cerr << "N: " << N << endl;
            cerr << "[ERROR] at (" << i << "," << j << ")" << endl;
            exit(-1);
          }
          p = (j * mWidth) + i;
          /*
          float Is = static_cast<float> (mState0[p]);
          float Iw = 0.0f;
          float Iv = 0.0f;
          getAttributesVW(&Iw, &Iv, 0, mAttrList[p], CACityExpansion::INHERITED);
//          float I = Is - (Iv * Iw);
          float I = -Iv * Iw;
*/
          float S = getAttributesVW(mAttrList[p], CACityExpansion::SUITABILITY, '+');
          float I = -getAttributesVW(mAttrList[p], CACityExpansion::INHERITED, '+');
          float E = getAttributesVW(mAttrList[p], CACityExpansion::ECOLOGICAL, '*');
          float P = getAttributesVW(mAttrList[p], CACityExpansion::PLANNING, '*');
          float V = 1.0f + powf(-log(random()), 0.25f);
          float NS = (S + aux + I) * E * P * V;

          //if ((i == 31) && (j == 32))
            //int nop = 42;

          if (NS < 0.0f)
            NS = 0.0f;
          if (NS > 100.0f)
            NS = 100.0f;
          float prob = random();
          if ((prob * 100.0f) < NS)
            mState1[p] = 100;
          else
            mState1[p] = mState0[p];

          if(mState1[p] < mState0[p])
            cerr << "#aqui" << endl;

/*

          if ((sum == 3) && (mState0[p] == 0))
              mState1[p] = 1;
          else if  ( ((sum >= 2) && (sum <= 3)) && (mState0[p] == 1))
              mState1[p] = 1;
          else
              mState1[p] = 0;
*/
      }//Edn-for (int i = 0; i < mWidth; i++){
   uint8_t *swap = mState0;
   mState0 = mState1;
   mState1 = swap;

}//end-void CACityExpansion::update(void){

void CACityExpansion::setDimension(int w, int h){
    mWidth = w;
    mHeight = h;
    assert((mWidth > 0) && (mHeight > 0));

    finalize();

    assert(posix_memalign((void**) &mState0, ALIGN,   mWidth *   mHeight *  sizeof(uint8_t)) == 0);
    assert(posix_memalign((void**) &mState1, ALIGN,   mWidth *   mHeight *  sizeof(uint8_t)) == 0);
    assert(posix_memalign((void**) &mAttrList, ALIGN,   mWidth *   mHeight *  sizeof(mAttrList)) == 0);
    memset(mState0, 0x00,   mWidth *   mHeight *  sizeof(uint8_t));
    memset(mState1, 0x00,   mWidth *   mHeight *  sizeof(uint8_t));
    memset(mAttrList, 0x00,   mWidth *   mHeight *  sizeof(mAttrList));
/*
    for (int j = 0; j < mHeight; j++){
      int p = j * mWidth;
      mState0[p] = mState1[p] = 100;
      mState0[p + mWidth - 1] = mState1[p + mWidth - 1] = 100;
    }

    for (int i = 0; i < mWidth; i++){
      int p = (mHeight - 1) * mWidth;
      mState0[i] = mState1[i] = 100;
      mState0[p+i] = mState1[p+i] = 100;
    }
*/

}


double  CACityExpansion::random(void){ return static_cast <double> (rand() % 65535 + 1) / 65535.0f; }

/*
 * Creates a list of attributes. Besides, each attribute may have one or more
 * attributes
 */
bool CACityExpansion::createAtt(const int i, const int j, const uint16_t def){
    stAttribList *ptr = NULL, *aux = NULL;
    int k = mWidth * j + i;
    assert(posix_memalign((void**) &ptr, ALIGN,  sizeof(stAttribList)) == 0);
    ptr->curr.values    = NULL;
    ptr->curr.weights   = NULL;
    ptr->curr.n        = 0;
    ptr->next          = NULL;
    ptr->def = def;


    if (mAttrList[k] == 0){
      mAttrList[k] = reinterpret_cast<uint64_t>(ptr);
      return true;
    }
    aux = reinterpret_cast<stAttribList*>(mAttrList[k]);

    while ( (aux->def != def) && (aux->next != NULL))
      aux = aux->next;

    //In this case, we already have the attribute - free memory and finish
    if (aux->def == def) {
      free(ptr);
      return false;
    }

    aux->next = ptr;
    return true;
}

/*
 * Sets values and weights of each attribute
 *
 */

bool CACityExpansion::addCityAttrib(const int i, const int j, const uint16_t def,
                                    const float *v, const float *w, const int n){
   int k = mWidth * j + i;
   stAttribList *ptr = reinterpret_cast<stAttribList*>(mAttrList[k]);

   while ( (ptr->def != def) && (ptr->next != NULL))
     ptr = ptr->next;

   if (ptr->def == def){
     float *my_w;
     float *my_v;
     assert(posix_memalign((void**) &my_w, ALIGN,  n * sizeof(float)) == 0);
     assert(posix_memalign((void**) &my_v, ALIGN,  n * sizeof(float)) == 0);

     memcpy(my_w, w,  n * sizeof(float));
     memcpy(my_v, v,  n * sizeof(float));
     ptr->curr.weights   = my_w;
     ptr->curr.values    = my_v;
     ptr->curr.n  = n;
     return true;
   }

    return false;

}


uint64_t CACityExpansion::getMemoryUsed(void){
  uint64_t total = 0;
  if (mState0 != NULL)
    total += (mWidth * mHeight * sizeof(uint8_t));

  if (mState1 != NULL)
    total += (mWidth * mHeight * sizeof(uint8_t));

  if (mAttrList != NULL){
    total += (mWidth * mHeight * sizeof(uint64_t));

    for (int k = 0; k < mWidth *   mHeight; k++){
      stAttribList *ptrA = reinterpret_cast<stAttribList*>(mAttrList[k]);
      while (ptrA != NULL){
        total += sizeof(stAttribList);
        total += sizeof(stCityAttr);
        total += (2 * ptrA->curr.n * sizeof(float)) + sizeof(int);


        ptrA = ptrA->next;

      }//end-while (ptrA != NULL){
    }//end-for (int k = 0; k < mWidth *   mHeight; k++){


  }//end-if (mAttrList != NULL){



  return total;
}


/*******************************************************************************
 *
 * Private methods
 *
 ******************************************************************************/
inline int CACityExpansion::min (int a, int b){
  if (a < b)
    return a;
  return b;
}

inline float CACityExpansion::distance(int x1, int x2, int y1, int y2){
  return static_cast<float>(abs(x1 - x2) + abs(y1 - y2));
}

inline void CACityExpansion::getAttributesVW(float *w, float *v, int idx, uint64_t list, uint16_t def){
  stAttribList *ptr = reinterpret_cast<stAttribList*>(list);
  if (ptr == NULL){
    *w  = *v = 0.0f;
    return ;
  }



  while ((ptr->next != NULL) && (ptr->def != def)){
    ptr = ptr->next;
  }//end-while (ptr->next != NULL) && (ptr->def != def){

  assert(ptr->def == def);
  assert(idx < ptr->curr.n);
  *w = ptr->curr.weights[idx];
  *v = ptr->curr.values[idx];


}


inline float CACityExpansion::getAttributesVW(uint64_t list, uint16_t def, char t){
  stAttribList *ptr = reinterpret_cast<stAttribList*>(list);
  if (ptr == NULL)
    return 0.0f;

  while ((ptr->next != NULL) && (ptr->def != def)){
    ptr = ptr->next;
  }//end-while (ptr->next != NULL) && (ptr->def != def){

  assert(ptr->def == def);

  float v = 0.0f;
  if (t == '*')
    v = 1.0f;

  for (int i = 0; i < ptr->curr.n; i++){
    if (t == '+')
      v += ( ptr->curr.values[i] * ptr->curr.weights[i]);
    else if (t == '*')
      v *= ( ptr->curr.values[i] * ptr->curr.weights[i]);
  }
  return v;
}

void CACityExpansion::debug(void){
  cout << "States: " << endl;
  for (int j = 0; j < mHeight; j++){
      cout << "\t";
   for (int i = 0; i < mWidth; i++){
     cout <<  setfill ('0')<< setw(3) << this->getState(i, j) << " ";
   }//end-for (int i = 0; i < WIDTH; i++){
     cout << endl;
  }//end-for (int j = 0; j < HEIGHT; j++){

}
