#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include <string>

float yaw=-90.0f,pitch=0.0f,lastX=400,lastY=300;
bool firstMouse=true,dragging=false;

float camX=0,camY=0,camZ=6;

struct CubeState{
    float rotX=50,rotY=30,rotZ=20;
    float scaleX=1,scaleY=1,scaleZ=1;
    bool wireframe=false;
    float colors[6][3]={{1,0,0},{0,1,0},{0,0,1},{1,1,0},{1,0,1},{0,1,1}};
} cube;

float rad(float d){return d*3.1415926f/180.0f;}

void mouse(GLFWwindow* w,double xpos,double ypos){
    if(!dragging){firstMouse=true;return;}
    if(firstMouse){lastX=xpos;lastY=ypos;firstMouse=false;}
    float xoff=xpos-lastX;
    float yoff=lastY-ypos;
    lastX=xpos;lastY=ypos;
    float sens=0.1f;
    xoff*=sens;yoff*=sens;
    yaw+=xoff; pitch+=yoff;
    if(pitch>89)pitch=89;
    if(pitch<-89)pitch=-89;
}

void input(GLFWwindow* w,float dt){
    float speed=5*dt;
    float fx=cos(rad(yaw))*cos(rad(pitch));
    float fy=sin(rad(pitch));
    float fz=sin(rad(yaw))*cos(rad(pitch));

    if(glfwGetKey(w,GLFW_KEY_W)==GLFW_PRESS){camX+=fx*speed;camY+=fy*speed;camZ+=fz*speed;}
    if(glfwGetKey(w,GLFW_KEY_S)==GLFW_PRESS){camX-=fx*speed;camY-=fy*speed;camZ-=fz*speed;}
    if(glfwGetKey(w,GLFW_KEY_A)==GLFW_PRESS){camX+=fz*speed;camZ-=fx*speed;}
    if(glfwGetKey(w,GLFW_KEY_D)==GLFW_PRESS){camX-=fz*speed;camZ+=fx*speed;}

    // Cube modifications
    if(glfwGetKey(w,GLFW_KEY_UP)==GLFW_PRESS) cube.rotX+=50*dt;
    if(glfwGetKey(w,GLFW_KEY_DOWN)==GLFW_PRESS) cube.rotX-=50*dt;
    if(glfwGetKey(w,GLFW_KEY_LEFT)==GLFW_PRESS) cube.rotY-=50*dt;
    if(glfwGetKey(w,GLFW_KEY_RIGHT)==GLFW_PRESS) cube.rotY+=50*dt;

    if(glfwGetKey(w,GLFW_KEY_Q)==GLFW_PRESS) cube.scaleX+=dt;
    if(glfwGetKey(w,GLFW_KEY_E)==GLFW_PRESS) cube.scaleX-=dt;
    if(glfwGetKey(w,GLFW_KEY_R)==GLFW_PRESS) cube.scaleY+=dt;
    if(glfwGetKey(w,GLFW_KEY_F)==GLFW_PRESS) cube.scaleY-=dt;
    if(glfwGetKey(w,GLFW_KEY_T)==GLFW_PRESS) cube.scaleZ+=dt;
    if(glfwGetKey(w,GLFW_KEY_G)==GLFW_PRESS) cube.scaleZ-=dt;

    if(glfwGetKey(w,GLFW_KEY_W)==GLFW_PRESS && glfwGetKey(w,GLFW_KEY_LEFT_SHIFT)==GLFW_PRESS) cube.wireframe=!cube.wireframe;
    if(glfwGetKey(w,GLFW_KEY_SPACE)==GLFW_PRESS){
        cube.rotX=50; cube.rotY=30; cube.rotZ=20;
        cube.scaleX=cube.scaleY=cube.scaleZ=1;
        cube.wireframe=false;
    }
}

void camera(){gluLookAt(camX,camY,camZ,camX+cos(rad(yaw))*cos(rad(pitch)),camY+sin(rad(pitch)),camZ+sin(rad(yaw))*cos(rad(pitch)),0,1,0);}

void drawCube(){
    if(cube.wireframe) glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); else glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glPushMatrix();
    glScalef(cube.scaleX,cube.scaleY,cube.scaleZ);
    glRotatef(cube.rotX,1,0,0);
    glRotatef(cube.rotY,0,1,0);
    glRotatef(cube.rotZ,0,0,1);

    glBegin(GL_QUADS);
    for(int i=0;i<6;i++) glColor3f(cube.colors[i][0],cube.colors[i][1],cube.colors[i][2]);
    // Front
    glVertex3f(-1,-1,1); glVertex3f(1,-1,1); glVertex3f(1,1,1); glVertex3f(-1,1,1);
    // Back
    glVertex3f(-1,-1,-1); glVertex3f(-1,1,-1); glVertex3f(1,1,-1); glVertex3f(1,-1,-1);
    // Top
    glVertex3f(-1,1,-1); glVertex3f(-1,1,1); glVertex3f(1,1,1); glVertex3f(1,1,-1);
    // Bottom
    glVertex3f(-1,-1,-1); glVertex3f(1,-1,-1); glVertex3f(1,-1,1); glVertex3f(-1,-1,1);
    // Right
    glVertex3f(1,-1,-1); glVertex3f(1,1,-1); glVertex3f(1,1,1); glVertex3f(1,-1,1);
    // Left
    glVertex3f(-1,-1,-1); glVertex3f(-1,-1,1); glVertex3f(-1,1,1); glVertex3f(-1,1,-1);
    glEnd();
    glPopMatrix();
}

void rect(float x,float y,float w,float h){glBegin(GL_QUADS);glVertex2f(x,y);glVertex2f(x+w,y);glVertex2f(x+w,y+h);glVertex2f(x,y+h);glEnd();}
void drawText(float x,float y,std::string s){glRasterPos2f(x,y);for(char c:s) glutBitmapCharacter(GLUT_BITMAP_8_BY_13,c);}

void panel(int w,int h){
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity(); glOrtho(0,w,0,h,-1,1);
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
    glDisable(GL_DEPTH_TEST);

    glColor3f(0,0,0); rect(10,h-200,300,190);
    glColor3f(0,1,0); rect(12,h-198,296,186);
    glColor3f(0,0,0); rect(15,h-195,290,180);

    glColor3f(0,1,0);
    drawText(20,h-30,"CUBE CONTROL PANEL");
    drawText(20,h-50,"Rotation X: "+std::to_string((int)cube.rotX));
    drawText(20,h-65,"Rotation Y: "+std::to_string((int)cube.rotY));
    drawText(20,h-80,"Rotation Z: "+std::to_string((int)cube.rotZ));
    drawText(20,h-95,"Scale X: "+std::to_string(cube.scaleX));
    drawText(20,h-110,"Scale Y: "+std::to_string(cube.scaleY));
    drawText(20,h-125,"Scale Z: "+std::to_string(cube.scaleZ));
    drawText(20,h-140,"Wireframe: "+std::string(cube.wireframe?"ON":"OFF"));
    drawText(20,h-160,"Use ARROWS to rotate");
    drawText(20,h-175,"Q/E,R/F,T/G to scale");
    drawText(20,h-190,"SHIFT+W toggle wireframe, SPACE reset");

    glEnable(GL_DEPTH_TEST);
    glPopMatrix(); glMatrixMode(GL_PROJECTION); glPopMatrix(); glMatrixMode(GL_MODELVIEW);
}

int main(){
    int argc=1; char* argv[1]={(char*)"App"}; glutInit(&argc,argv);
    glfwInit();
    GLFWwindow* win=glfwCreateWindow(1000,700,"Extreme NASA Cube",0,0);
    glfwMakeContextCurrent(win);

    glfwSetCursorPosCallback(win,mouse);
    glfwSetMouseButtonCallback(win,[](GLFWwindow* w,int button,int action,int mods){if(button==GLFW_MOUSE_BUTTON_LEFT) dragging=(action==GLFW_PRESS);});
    glfwSetInputMode(win,GLFW_CURSOR,GLFW_CURSOR_NORMAL);
    glEnable(GL_DEPTH_TEST);

    double last=glfwGetTime();
    while(!glfwWindowShouldClose(win)){
        double now=glfwGetTime(); float dt=now-last; last=now;
        input(win,dt);

        int w,h; glfwGetFramebufferSize(win,&w,&h);
        glViewport(0,0,w,h);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION); glLoadIdentity(); gluPerspective(45.0,(double)w/h,0.1,100);
        glMatrixMode(GL_MODELVIEW); glLoadIdentity();
        camera();

        drawCube();
        panel(w,h);

        glfwSwapBuffers(win); glfwPollEvents();
    }
    glfwTerminate();
}