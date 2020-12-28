#include <iostream>
#include <ogl/glew.h>
#include <ogl/freeglut.h>

#include "Shaders.h"
#include "Model.h"

#define I glm::mat4(1.0)

#define opacity 0.75

void funInit();
void funReshape(int w, int h);
void funDisplay();


void drawObject(Model model, glm::vec3 color, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawSuelo(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawAspa(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawHelice(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawSoporte(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawArticulacion(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawCabina(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawBrazo(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawBrazoHelice(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawBrazos(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawEsfera(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawEstructuraSuperior(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawTronco(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawPlataforma(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawTiovivo(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void funTimer(int value);
void funKeyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void funSpecial(int key, int x, int y);
void mouseMove(int x, int y);


// Shaders
Shaders shaders;

// Modelos
Model plane;
Model cylinder;
Model sphere;
Model cone;

//fovy
float fovy = 30.0;

// Viewport
int w = 600;
int h = 600;

//Cámara
float alphaX = 0.0;
float alphaY = 0.0;
float move = 0.0;

// Transformaciones
float desYCone = -2.75455951691;
float desYCylinder = -1;
float DesXHelice = 1;
float DesYHelice = -0.3;
float desZCylinder = -1;
float desYPlataforma = 1;
float desYTronco = 2.1;
float desYEstructuraSuperior = 1.30;
float rotX = 90.0;
float rotY90 = 90.0;
float rotZ90 = 90.0;
float rotY45 = 45.0;
float rotY72 = 72.0;
float rotHelice = 0;
float rotTiovivo = 0;
float desZtiovivo = 0.0;
float desXtiovivo = 0.0;
float rotBrazo = 0.0;

//Tiempo
GLint speed = 20;


int main(int argc, char** argv) {

    // Inicializamos GLUT
    glutInit(&argc, argv);
    glutInitContextVersion(3,3);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(w,h);
    glutInitWindowPosition(50,50);
    glutCreateWindow("Practica 2");

    // Inicializamos GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(GLEW_OK != err) {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
        return 0;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    const GLubyte *oglVersion = glGetString(GL_VERSION);
    std::cout <<"This system supports OpenGL Version: " << oglVersion << std::endl;

    // Inicializaciones específicas
    funInit();

    // Configuración CallBacks
    glutReshapeFunc(funReshape);
    glutDisplayFunc(funDisplay);
    glutTimerFunc(speed,funTimer,0);
    glutKeyboardFunc(funKeyboard);
    glutMouseFunc(mouse);
    glutSpecialFunc(funSpecial);
    glutMotionFunc(mouseMove);

    // Bucle principal
    glutMainLoop();

    return 0;
}

void funInit() {

    // Test de profundidad
    glEnable(GL_DEPTH_TEST);
    glPolygonOffset(1.0,1.0);

    // Shaders
    shaders.initShaders("resources/shaders/vshader.glsl","resources/shaders/fshader.glsl");

    // Modelos
    plane.initModel("resources/models/plane.obj");
    cylinder.initModel("resources/models/cylinder.obj");
    sphere.initModel("resources/models/sphere.obj");
    cone.initModel("resources/models/cone.obj");


}

void funReshape(int wnew, int hnew) {

    // Configuración del Viewport
    glViewport(0, 0, wnew, hnew);

    // Actualización de w y h
    w = wnew;
    h = hnew;

}

void funDisplay() {

    // Borramos el buffer de color
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Indicamos los shaders a utilizar
    shaders.useShaders();

    // Matriz P
    float nplane =  0.1;
    float fplane = 25.0;
    float aspect = (float)w/(float)h;
    glm::mat4 P  = glm::perspective(glm::radians(fovy), aspect, nplane, fplane);

    // Matriz V
    float x = 5.0f*glm::cos(glm::radians(alphaY))*glm::sin(glm::radians(alphaX));
    float y = 5.0f*glm::sin(glm::radians(alphaY));
    float z = 5.0f*glm::cos(glm::radians(alphaY))*glm::cos(glm::radians(alphaX));
    glm::vec3 pos(x,y,z);
    glm::vec3 eye(0.0, 0.0, 0.0);
    glm::vec3 up    (0.0, 1.0,  0.0);
    glm::mat4 V = glm::lookAt(pos, eye, up);

    // Dibujamos la escena
    drawSuelo(P,V,I);
    glm::mat4 T = glm::translate(I, glm::vec3(desXtiovivo, 0.0, desZtiovivo));
    drawTiovivo(P,V,T);


    // Intercambiamos los buffers
    glutSwapBuffers();

}

void drawObject(Model model, glm::vec3 color, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    shaders.setMat4("uPVM",P*V*M);

    glEnable(GL_POLYGON_OFFSET_FILL);
    shaders.setVec3("uColor",color);
    model.renderModel(GL_FILL);
    glDisable(GL_POLYGON_OFFSET_FILL);
    glm::vec3 colorNew = color.operator*=(opacity);
    shaders.setVec3("uColor",colorNew);
    model.renderModel(GL_LINE);

}

void drawSuelo(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 S = glm::scale(I, glm::vec3(2.0, 1.0, 2.0));
    drawObject(plane,glm::vec3(0.552941 , 0.552941 , 0.552941),P,V,M*S);

}

void drawAspa(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 S = glm::scale(I, glm::vec3(0.037, 0.06,  0.011 ));
    glm::mat4 R = glm::rotate   (I, glm::radians(rotX), glm::vec3(1, 0, 0));
    glm::mat4 T = glm::translate(I, glm::vec3(0.0, desYCone, 0.0));
    drawObject(cone,glm::vec3(1.0, 0.0, 0.0),P,V,M*R*S*T);

}

void drawHelice(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 RY90 = glm::rotate(I, glm::radians(rotY90), glm::vec3(0, 1, 0));
    drawAspa(P,V,M);
    drawAspa(P,V,M*RY90);
    drawAspa(P,V,M*RY90*RY90);
    drawAspa(P,V,M*RY90*RY90*RY90);

}

void drawSoporte(glm::mat4 P, glm::mat4 V, glm::mat4 M){
    glm::mat4 S = glm::scale(I, glm::vec3(0.025, 0.15,  0.025));
    glm::mat4 T = glm::translate(I, glm::vec3(0.0, -0.15, 0.0));
    drawObject(cylinder,glm::vec3(0.0, 1.0, 1.0),P,V,M*T*S);
}


void drawArticulacion(glm::mat4 P, glm::mat4 V, glm::mat4 M){
    glm::mat4 S = glm::scale(I, glm::vec3(0.075, 0.075,  0.075));
    drawObject(sphere,glm::vec3(1.0, 0.0, 1.0),P,V,M*S);

}


void drawBrazo(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    glm::mat4 S = glm::scale(I, glm::vec3(0.05, 0.5,  0.05));
    glm::mat4 R = glm::rotate   (I, glm::radians(rotX), glm::vec3(1, 0, 0));
    glm::mat4 T = glm::translate(I, glm::vec3(0.0, desYCylinder, desZCylinder));
    drawObject(cylinder,glm::vec3(0.0, 0.0, 1.0),P,V,M*R*S*T);
}

void drawCabina(glm::mat4 P, glm::mat4 V, glm::mat4 M){
    glm::mat4 TX = glm::translate(I, glm::vec3(1, 0.0,0.0));
    drawSoporte(P,V,M*TX);

    //Matriz de rotación que mantendrá girando las helices del molino de forma automática.
    glm::mat4 RY = glm::rotate(I, glm::radians(rotHelice), glm::vec3(0, 1, 0));
    glm::mat4 T = glm::translate(I, glm::vec3(DesXHelice, DesYHelice,0.0));
    drawHelice(P,V,M*T*RY);
}

void drawBrazoHelice(glm::mat4 P, glm::mat4 V, glm::mat4 M){

    float coseno = glm::cos(glm::radians(rotBrazo));
    float seno = glm::sin(glm::radians(rotBrazo));
    //Matriz de traslación que posiciona la esfera magenta al final del cilindro azul principal.
    glm::mat4 TX = glm::translate(I, glm::vec3(1, 0.0,0.0));
    //Matriz de rotación que rota el brazo completo el ángulo correspondiente.
    glm::mat4 R = glm::rotate(I, glm::radians(rotBrazo), glm::vec3(0, 0, -1));
    /*Matriz uno de traslación que lleva el eje a la posición origen,
    para después poder rotar el cilindro y la helice y mantenerlos a 90º.*/
    glm::mat4 TC1 = glm::translate(I, glm::vec3(-coseno, seno,0.0));
    //Matriz de rotación para mantener el cilindro y la hélice con un ángulo de 90º
    glm::mat4 RInversa = glm::rotate(I, glm::radians(rotBrazo), glm::vec3(0, 0, 1));
    //Matriz dos de traslación que lleva el eje a la posición inicial.
    glm::mat4 TC2 = glm::translate(I, glm::vec3(coseno, -seno,0.0));

    drawArticulacion(P,V,M*R*TX);

    drawCabina(P,V,M*TC2*RInversa*TC1*R);

    glm::mat4 RY90 = glm::rotate(I, glm::radians(rotY90), glm::vec3(0, -1, 0));
    glm::mat4 RY902 = glm::rotate(I, glm::radians(rotZ90), glm::vec3(0, 0, 1));
    glm::mat4 T2 = glm::translate(I, glm::vec3(0.0, -0.05,0.0));
    drawBrazo(P,V,M*R*RY90*RY902*T2);

}

void drawBrazos(glm::mat4 P, glm::mat4 V, glm::mat4 M){
    glm::mat4 RY72 = glm::rotate(I, glm::radians(rotY72), glm::vec3(0, 1, 0));
    drawBrazoHelice(P,V,M);
    drawBrazoHelice(P,V,M*RY72);
    drawBrazoHelice(P,V,M*RY72*RY72);
    drawBrazoHelice(P,V,M*RY72*RY72*RY72);
    drawBrazoHelice(P,V,M*RY72*RY72*RY72*RY72);

}

void drawEsfera(glm::mat4 P, glm::mat4 V, glm::mat4 M){
    glm::mat4 S = glm::scale(I, glm::vec3(0.3, 0.3,  0.3));
    drawObject(sphere,glm::vec3(0.0, 1.0, 0.0),P,V,M*S);
}

void drawEstructuraSuperior(glm::mat4 P, glm::mat4 V, glm::mat4 M){
    drawBrazos(P,V,M);
    drawEsfera(P,V,M);

}

void drawTronco(glm::mat4 P, glm::mat4 V, glm::mat4 M){
    glm::mat4 S = glm::scale(I, glm::vec3(0.093,0.268,0.093));
    //Lo desplazamos para ponerlo apoyado justo en el suelo.
    glm::mat4 T = glm::translate(I, glm::vec3(0.0, desYTronco,0.0));
    drawObject(cone,glm::vec3(1.0, 0.5, 0.0),P,V,M*S*T);
}

void drawPlataforma(glm::mat4 P, glm::mat4 V, glm::mat4 M){
    glm::mat4 S = glm::scale(I, glm::vec3(0.6,0.05,0.6));
    //Lo desplazamos para ponerlo apoyado justo en el suelo.
    glm::mat4 T = glm::translate(I, glm::vec3(0.0, desYPlataforma,0.0));
    drawObject(cylinder,glm::vec3(1.0, 1.0, 0.0),P,V,M*S*T);
}

void drawTiovivo(glm::mat4 P, glm::mat4 V, glm::mat4 M){
    drawPlataforma(P,V,M);
    //Matriz para rotar  el tiovivo excepto la plataforma
    glm::mat4 R = glm::rotate(I, glm::radians(rotTiovivo), glm::vec3(0, 1, 0));
    drawTronco(P,V,M*R);
    glm::mat4 T = glm::translate(I, glm::vec3(0.0, desYEstructuraSuperior, 0.0));
    drawEstructuraSuperior(P,V,M*T*R);
}


void funTimer(int value) {
    rotHelice+=2.5;
    glutPostRedisplay();
    glutTimerFunc(speed,funTimer,0);

}

void funKeyboard(unsigned char key, int x, int y) {

    switch(key) {
        case 'r':
            rotTiovivo-=5;
            break;
        case 'R':
            rotTiovivo+=5;
            break;
        case 'Y':
            if(desYEstructuraSuperior<=1.29) {
                desYEstructuraSuperior+=0.01;
            }
            break;
        case 'y':
            if(desYEstructuraSuperior>=0.34){
                desYEstructuraSuperior-=0.01;
            }
            break;
        case 'a':
            if(rotBrazo<45) {
                rotBrazo+=3;
            }
            break;
        case 'A':
            if(rotBrazo>0){
                rotBrazo-=3;
            }
            break;

    }
    glutPostRedisplay();

}

void mouse(GLint button, GLint state, GLint x, GLint y){

    //Si estamos haciendo scroll hacia arriba, el fovy dismuye.
    if(button == 3){
        if(fovy>10) {
            fovy -= 1;
        }
        //Si estamos haciendo scroll hacia abajo, el fovy aumenta.
    }else if(button == 4){
        if(fovy<60){
            fovy+=1;
        }
    }

    //Si y solo si tenemos pulsado el botón izquierdo del ratón, la posición de la cámara se mueve.
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ){
        move = 1.0;
    } else {
        move = 0.0;
    }
}

void funSpecial(int key, int x, int y) {

    //El límite (1.4) viene calculado por la diferencia entre el tamaño del plano (2) y la mitad de la plataforma (0.6).
    switch(key) {
        case GLUT_KEY_UP:
            if(desZtiovivo>-1.4){
                desZtiovivo -= 0.1;
            }
            break;
        case GLUT_KEY_DOWN:
            if(desZtiovivo<1.4){
                desZtiovivo += 0.1;
            }
            break;
        case GLUT_KEY_LEFT:
            if(desXtiovivo>-1.4){
                desXtiovivo -= 0.1;
            }
            break;
        case GLUT_KEY_RIGHT:
            if(desXtiovivo<1.4){
                desXtiovivo += 0.1;
            }
            break;
    }
    glutPostRedisplay();

}

void mouseMove(int x, int y){
    //Colocamos la posición recibida del puntero del ratón respecto al centro de la ventana.
    x -= 300;
    y -= 300;

    /*Para comprobar coordenadas X,Y
    std::cout <<"---------------------------------" << std::endl;
    std::cout <<"X:"<< x << std::endl;
    std::cout <<"Y:"<< y << std::endl;
    std::cout <<"---------------------------------" << std::endl;*/

    if(move == 1){

        //Calculamos el angulo de movimiento
        float auxAlphaX = (x*90.0)/300;
        /*Hay que calcular el opuesto, puesto que, originalmente, la Y que nos devolvían tenía las Y positivas hacia
         abajo y nosotros queremos lo contrario; cuando el puntero se mueva hacia arriba, la cámara vaya hacia arriba*/
        float auxAlphaY = -((y*90.0)/300);

        /*si el ángulo calculado respecto a la posición del ratón está dentro del límite, lo actualizamos,
         para mover la cámara. Si no, se mantiene como esta.*/
        if((auxAlphaX>=-179 && auxAlphaX<=179) && (auxAlphaY>=-89 && auxAlphaY<=89)){
            alphaX = auxAlphaX;
            alphaY = auxAlphaY;
        }
    }
}









