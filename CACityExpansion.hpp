#ifndef _CACITYEXPANSION_HPP_
#define _CACITYEXPANSION_HPP_
#include <App.hpp>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstdint>
#define STRING_SIZE 64

using namespace std;
/*
 * stCityAttr represents the attribute. It is composed by values and associated
 * weight
 */
struct stCityAttr{
    float *weights;
    float *values;
    int n;
};

/*
 * There are list of attributes. One list per cell.
 * Each element of list is a point to values, definition
 * and a pointer to another set of values in next level
 */
struct stAttribList{
  uint16_t def;                           /* definition of attribute */
  stCityAttr curr;                        /* current attribute */
  stAttribList *next;                     /* Next attribute of list */
};


/*
 * CACityExpansion is UES -> Urban Expansion Scenarios
 */
class CACityExpansion{
public:
  static const uint8_t FREE = 0;
  static const uint8_t OCCUPAIED = 100;

  /* These are cell attributes 
   * It is not necessary all atribtutes in all cells
   * We adopted an ideia of attribure classes - 4 classes as following: */
   
  static const uint16_t SUITABILITY = 0x8000; 
  static const uint16_t INHERITED   = 0x8001;
  static const uint16_t ECOLOGICAL  = 0x8002;
  static const uint16_t PLANNING    = 0x8003;
  
public:
    CACityExpansion();
    ~CACityExpansion();
    void help(void);
    virtual void init(void);
    virtual void update(void);
    int  getState(int, int);
    void setState(int, int, uint8_t);
    virtual void finalize(void);
    bool isRunning (void)     {return mRunning; };
    bool isInit(void)         { return mInit; };
    void setDimension(int, int);
    void offRunning(void)     { mRunning = false;};
    void onRunning(void)      { mRunning = true;};
    double random(void);

    bool createAtt(const int , const int , const uint16_t );
    bool addCityAttrib(const int i, const int j, const uint16_t def,
                                    const float *w, const float *v, const int n);

    uint64_t getMemoryUsed(void);
    void debug(void);

private:
    int min(int, int);
    float distance(int, int, int, int);
    void getAttributesVW(float *w, float *v, int idx, uint64_t list, uint16_t def);
    float getAttributesVW(uint64_t, uint16_t, char);


protected:
  bool                               mInit,
                                     mRunning;

  int                                mWidth,
                                     mHeight,
                                     mRadius;


  uint8_t                           *mState0,
                                    *mState1;

  uint64_t                          *mAttrList;



};
#endif
