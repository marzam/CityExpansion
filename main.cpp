#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <CACityExpansion.hpp>
#define WIDTH  10000
#define HEIGHT 10000
#define TIME   10
using namespace std;
int main (int ac, char **av){
  CACityExpansion *CA = new CACityExpansion();
  cout << "City expasion simulation" << endl;
//-----------------------------------------------------------------------------
  CA->setDimension(WIDTH, HEIGHT);
  CA->init();
  //Creating attributes
  for (int j = 0; j < HEIGHT; j++){
    for (int i = 0; i < WIDTH; i++){
     CA->createAtt(i, j, CACityExpansion::SUITABILITY);
     CA->createAtt(i, j, CACityExpansion::INHERITED);
     CA->createAtt(i, j, CACityExpansion::ECOLOGICAL);
     CA->createAtt(i, j, CACityExpansion::PLANNING);

    }//end-for (int i = 0; i < WIDTH; i++){
  }//end-for (int j = 0; j < HEIGHT; j++){
  //------------------------------------------------------------------------------
  //each v parameter is suitability value and its weights
  float sw[] = {0.25f, 0.125f, 0.125f};
  float sv[] = {100.0f, 100.0f, 100.0f};

  //Inherited through time
  float iw[] = {1.5f};
  float iv[] = {100.0f};


  //Ecological constraint is if possible expasion to cell or not
  //Each position is a constraint such as rivers, mountains and so on
  //Following by weights
  float ew[] = {1.0f, 1.0f, 1.0f};
  float ev[] = {1.0, 1.0f, 1.0f};

  //Planning is goverment planing for that cell
  //As the input is an array, each position may represent a planning attribute
  //Following by weights
  float pw[] = {1.0f, 1.0f, 1.0f};
  float pv[] = {1.0, 1.0f, 1.0f};

  for (int j = 0; j < HEIGHT; j++){
   for (int i = 0; i < WIDTH; i++){
      CA->addCityAttrib(i, j, CACityExpansion::SUITABILITY, &(sv[0]), &(sw[0]), 3);
      CA->addCityAttrib(i, j, CACityExpansion::INHERITED, &(iv[0]), &(iw[0]), 1);
      CA->addCityAttrib(i, j, CACityExpansion::ECOLOGICAL, &(ev[0]), &(ew[0]), 3);
      CA->addCityAttrib(i, j, CACityExpansion::PLANNING, &(pv[0]), &(pw[0]), 3);
    
   }//end-for (int i = 0; i < WIDTH; i++){
  }//end-for (int j = 0; j < HEIGHT; j++){
/*
  cout << "States: " << endl;
  for (int j = 0; j < HEIGHT; j++){
      cout << "\t";
   for (int i = 0; i < WIDTH; i++){
     cout <<  CA->getState(i, j) << " ";
   }//end-for (int i = 0; i < WIDTH; i++){
     cout << endl;
  }//end-for (int j = 0; j < HEIGHT; j++){
*/
  CA->onRunning();
  for (int t = 0; t < TIME; t++)
    CA->update();

  /*
  cout << "States: " << endl;
    for (int j = 0; j < HEIGHT; j++){
        cout << "\t";
     for (int i = 0; i < WIDTH; i++){
       cout << setw(4) <<  CA->getState(i, j) << " ";
     }//end-for (int i = 0; i < WIDTH; i++){
       cout << endl;
    }//end-for (int j = 0; j < HEIGHT; j++){
*/
  cout << "END OF MESSAGE" << endl;
  return EXIT_SUCCESS;
}
