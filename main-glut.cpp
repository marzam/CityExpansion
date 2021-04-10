/*
 * File Main
 * Traffic Cellular Automata simulation  TCA-S
 */

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <cstdlib>
#include <cstring>
#include <App.hpp>
#include <cstdio>
#include <CACityExpansion.hpp>

#define WIDTH  200
#define HEIGHT 250

Stopwatch stopwatch;

float     elapsedTime = 0.0f,
          FPS = 0.0f;

int       width = 800,
          height = 600;

bool      start = false;


float    mScaleX = 0.0f,
         mScaleY = 0.0f;

CACityExpansion *CA = new CACityExpansion();

void drawCell(float x, float y){
    glPushMatrix();
    glTranslatef(x, y, 0.0f);

    glBegin(GL_QUADS); //GL_QUADS);GL_LINE_LOOP

    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, -mScaleY, 0.0f);
    glVertex3f(mScaleX, -mScaleY, 0.0f);
    glVertex3f(mScaleX, 0.0f, 0.0f);


    glEnd();
    glPopMatrix();
}
void render(void)
{
    glClear(GL_COLOR_BUFFER_BIT);


//------------------------------------------------------------------------------
  if (CA->isInit()){
    float dX = 0.0f,
          dY = 0.0f;



    for (int j = 0; j < HEIGHT; j++){
        for (int i = 0; i <  WIDTH; i++){
            glColor3f(0.0f, 0.0f, 0.0f);
            switch (CA->getState(i,j)){
              case CACityExpansion::FREE:glColor3f(0.2f, 0.8f, 0.2f);break;
              case CACityExpansion::OCCUPAIED:glColor3f(0.8f, 0.2f, 0.2f);break;

            }
            drawCell(dX, dY);
            dX += mScaleX;
        }

        dY -= mScaleY;
        dX = 0.0f;

      }

    }
//------------------------------------------------------------------------------

    glutSwapBuffers();


}

// Função de inicialização de parâmetros (OpenGL e outros)
void init (void)
{
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);



   mScaleY  = 1.0f / static_cast <float> (HEIGHT);
   mScaleX  = 1.0f / static_cast <float> (WIDTH);


   //-----------------------------------------------------------------------------
     CA->setDimension(WIDTH, HEIGHT);
     CA->init();
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
     float sw[] = {1.0f/3.0f, 1.0f/3.0f, 1.0f/3.0f};
     float sv[] = {100.0f, 100.0f, 100.0f};

     //Inherited through time
     float iw[] = {1.163f};
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
         if ((j != (HEIGHT / 4)) || (i != (WIDTH / 2)))
            CA->addCityAttrib(i, j, CACityExpansion::ECOLOGICAL, &(ev[0]), &(ew[0]), 3);
         else{
           float ewaux[] = {1.0f};
           float evaux[] = {0.0f};
           CA->addCityAttrib(i, j, CACityExpansion::ECOLOGICAL, &(evaux[0]), &(ewaux[0]), 1);
         }
         CA->addCityAttrib(i, j, CACityExpansion::PLANNING, &(pv[0]), &(pw[0]), 3);

      }//end-for (int i = 0; i < WIDTH; i++){
     }//end-for (int j = 0; j < HEIGHT; j++){

    CA->setState((WIDTH/2) + (WIDTH/4), HEIGHT/2, CACityExpansion::OCCUPAIED);
    CA->setState(WIDTH/2, HEIGHT/2, CACityExpansion::OCCUPAIED);
//CA->debug();
}

// Função de evento do teclado
void keyboardEvent(unsigned char key, int x, int y)
{
     //    glutPostRedisplay();
    switch (key) {
        case 'u':
        case 'U':
          if (CA->isInit()) CA->update();
          glutPostRedisplay();
          break;
        case 'm':
        case 'M':
          cout << "Memory used: " << CA->getMemoryUsed() << " bytes" << endl;
        case 'i':
        case 'I':
            break;

        case 'r':
        case 'R':
            CA->onRunning();
            cout << "Start running.." << endl;
            break;

        case 'q':
        case 'Q':
        case 27:
            CA->finalize();
            exit (EXIT_SUCCESS);
            break;

        case 'p':
        case 'P':
          CA->debug();
          break;

        default:
            break;
   }
}

// Função de evento do mouse
void mouseEvent(int button, int state, int x, int y)
{
/*
    if (button == GLUT_LEFT_BUTTON)
        if (state == GLUT_DOWN){
            float x1 = (static_cast<float>(x) / width) * static_cast<float>(cellularAutomata->getCellX());
            float y1 = (static_cast<float>(y) / height) * static_cast<float>(cellularAutomata->getCellY());
            int x2 = static_cast<int>(x1)  ;
            int y2 = static_cast<int>(y1) ;
            //x2 /=  cellularAutomata->getCellX();
            //y2 /=  cellularAutomata->getCellY();

            //cellularAutomata->changeState(x2, y2);

        }

    if (button == GLUT_RIGHT_BUTTON)
        if (state == GLUT_DOWN)
            cerr << "Right button" << endl;
*/
   glutPostRedisplay();
}

//Viewport
void viewPort(int w, int h)
{

    if(h == 0) h = 1;


    width = w;
    height = h;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho (-0.01f, 1.01f, -1.01f, 0.01f, -1.0f, 1.0f);
    glutPostRedisplay();
}

//Loop principal da visualização
void mainloop(void)
{
  STOP_STOPWATCH(stopwatch);
    elapsedTime += stopwatch.mElapsedTime;

    //

    FPS++;
    if (FPS >= 100.0f){
        float realfps = 1000.0f / (elapsedTime / 100.0f);
        char msg[1024];
        sprintf(msg, "CA - Simulation FPS: %.2f", realfps);
        glutSetWindowTitle(msg);
        FPS = 0.0f;
        elapsedTime = 0.0f;
    }
    START_STOPWATCH(stopwatch);

    if (CA->isInit()) CA->update();

    glutPostRedisplay();
}


int main(int argc, char**argv)
{
    START_STOPWATCH(stopwatch);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutCreateWindow("Cellular Automata - City Simulation - v1.0");
    glutDisplayFunc(render);
    glutReshapeFunc(viewPort);
    glutMouseFunc(mouseEvent);
    glutKeyboardFunc(keyboardEvent);
    glutIdleFunc(mainloop);
    init();
    glutMainLoop();
    return EXIT_SUCCESS;
}
